#pragma once

#include <memory>
#include <mutex>

namespace nierika::utils
{

    template <typename T>
    class Singleton
    {
    public:
        static T& getInstance()
        {
            std::call_once(initFlag, []() {
                instance.reset(new T());
            });
            return *instance;
        }
    
        Singleton(Singleton const&) = delete;
        void operator=(Singleton const&) = delete;
    
    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
    
    private:
        static std::unique_ptr<T> instance;
        static std::once_flag initFlag;
    };

    template <typename T>
    std::unique_ptr<T> Singleton<T>::instance = nullptr;
    
    template <typename T>
    std::once_flag Singleton<T>::initFlag;
}
