#pragma once

#include <mutex>
#include <memory>

template <typename T>
struct Singleton
{
public:
    T& GetInstance() const
    {
        std::call_once(flag, [this] { instance.reset(new T()); });
        return *instance;
    }

private:
    std::once_flag flag;
    std::unique_ptr<T> instance;
};
