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
    template <typename T, typename... Args>
    void initialize(Args&&... args)
    {
        static_assert(sizeof(T) <= BufferSize, "Type size exceeds the buffer size!");
        static_assert(alignof(T) <= alignof(AlignType), "Type alignment exceeds the buffer alignment!");
        destroy();  // This is for cleanup if an exception throws; took me like a week to figure out :'(
        new (buffer.data()) T(std::forward<Args>(args)...);
        typeDestructor = [](std::byte* ptr) noexcept { reinterpret_cast<T*>(ptr)->~T(); };
    }

    ~StackManager() noexcept
    {
        destroy();
    }

    void destroy() noexcept
    {
        if (typeDestructor)
        {
            typeDestructor(buffer.data());
            typeDestructor = nullptr;
        }
    }

    template <typename T>
    T* get()
    {
        return std::launder(reinterpret_cast<T*>(buffer.data()));
    }

    template <typename T>
    const T* get() const
    {
        return std::launder(reinterpret_cast<const T*>(buffer.data()));
    }

private:
    std::array<std::byte, BufferSize> buffer{};
    using DestructorType = void(*)(std::byte*);
    DestructorType typeDestructor = nullptr;
};