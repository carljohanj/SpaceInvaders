#pragma once
#include <array>
#include <cstddef>
#include <new>
#include <utility>
#include <type_traits>
#include <cassert>
#include <memory>

template <std::size_t BufferSize = 256, typename AlignType = std::max_align_t>
class StackManager
{
public:
    template <typename Type, typename... Args>
    void initialize(Args&&... args)
    {
        static_assert(sizeof(Type) <= BufferSize, "Type size exceeds the buffer size!");
        static_assert(alignof(Type) <= alignof(AlignType), "Type alignment exceeds the buffer alignment!");
        
        destroy();
        new (buffer.data()) Type(std::forward<Args>(args)...);
        typeDestructor = [] (std::byte* ptr) noexcept { std::launder(reinterpret_cast<Type*>(ptr))->~Type(); };
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
    Type* get()
    {
        return std::launder(reinterpret_cast<Type*>(buffer.data()));
    }

    template <typename Type>
    const Type* get() const
    {
        return std::launder(reinterpret_cast<const Type*>(buffer.data()));
    }

private:
    std::array<std::byte, BufferSize> buffer{};
    using DestructorType = void(*)(std::byte*);
    DestructorType typeDestructor = nullptr;
};