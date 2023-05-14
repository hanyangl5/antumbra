#pragma once

// standard libraries
#include <memory>
#include <type_traits>

// third party libraries

// project headers

namespace ant {

template<typename T> class Singleton
{
  public:
    Singleton() = default;
    virtual ~Singleton() noexcept = default;

    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton &operator=(const Singleton &) = delete;
    Singleton &operator=(Singleton &&) = delete;

    static T &get() noexcept(std::is_nothrow_constructible<T>::value)
    {
        static T instance;
        return instance;
    }
};

}// namespace ant