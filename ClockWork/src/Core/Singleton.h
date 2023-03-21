#pragma once
#include <memory>


template<typename T>
class Singleton {
public:
    static T& Instance();

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    struct token {};
    Singleton() {}
};

template<typename T>
T& Singleton<T>::Instance()
{
    static T instance{ token{} };
    return instance;
}
