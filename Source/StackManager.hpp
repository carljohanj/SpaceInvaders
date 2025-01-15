#pragma once
#include <array>
#include <cassert>
#include <cstddef>
#include <gsl/gsl>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>


template <std::size_t BufferSize = 256, typename AlignType = std::max_align_t>
class StackManager
{
public:
    StackManager() = default;
    StackManager(const StackManager& other) = delete;
    StackManager& operator=(const StackManager& other) = delete;
    StackManager(StackManager&& other) noexcept { moveFrom(std::move(other)); }
    StackManager& operator=(StackManager&& other) noexcept
    {
        destroy();
        moveFrom(std::move(other));
        return *this;
    }

    template <typename Type, typename... Args>
    void initialize(Args&&... args)
    {
        static_assert(sizeof(Type) <= BufferSize, "Type size exceeds the buffer size!");
        static_assert(alignof(Type) <= alignof(AlignType), "Type alignment exceeds the buffer alignment!");

        destroy();
        new (buffer.data()) Type(std::forward<Args>(args)...);

        [[gsl::suppress(type .1, justification: "We have to use reinterpret_cast, but we can make it compiler safe with std::launder")]]
        typeDestructor = [](std::byte* ptr) noexcept { std::destroy_at(std::launder(reinterpret_cast<Type*>(ptr))); };
    }

    ~StackManager() noexcept { destroy(); }

    void destroy() noexcept
    {
        if (typeDestructor)
        {
            typeDestructor(buffer.data());
            typeDestructor = nullptr;
        }
    }

    template <typename Type>
    Type* get() noexcept
    {
        [[gsl::suppress(type .1, justification: "We have to use reinterpret_cast, but we can make it compiler safe with std::launder")]]
        return std::launder(reinterpret_cast<Type*>(buffer.data()));
    }

    template <typename Type>
    const Type* get() const noexcept
    {
        [[gsl::suppress(type .1, justification: "We have to use reinterpret_cast, but we can make it compiler safe with std::launder")]]
        return std::launder(reinterpret_cast<const Type*>(buffer.data()));
    }

    void moveFrom(StackManager&& other) noexcept
    {
        buffer = std::move(other.buffer);
        typeDestructor = other.typeDestructor;
        other.typeDestructor = nullptr;
    }

private:
    std::array<std::byte, BufferSize> buffer{};
    using DestructorType = void(*)(std::byte*);
    DestructorType typeDestructor = nullptr;
};