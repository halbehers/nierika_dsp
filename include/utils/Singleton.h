#pragma once

#include <memory>
#include <mutex>
#include <utility>

namespace nierika::utils
{

    template <typename T>
    class Singleton
    {
    public:
        template <typename... Args>
        static T& getInstance(Args&&... args)
        {
            std::call_once(initFlag, [&]() {
                instance.reset(new T(std::forward<Args>(args)...));
                alive.store(true, std::memory_order_release);
            });
            return *instance;
        }

        static bool isAlive()
        {
            return alive.load(std::memory_order_acquire);
        }
    
        Singleton(Singleton const&) = delete;
        void operator=(Singleton const&) = delete;


        static void markShuttingDown()
        {
            alive.store(false, std::memory_order_relaxed);
        }
    
    protected:
        Singleton() = default;
        virtual ~Singleton()
        {
            alive.store(false, std::memory_order_release);
        }
    
    private:
        static std::unique_ptr<T> instance;
        static std::once_flag initFlag;
        static std::atomic<bool> alive;
    };

    template <typename T>
    std::unique_ptr<T> Singleton<T>::instance = nullptr;
    
    template <typename T>
    std::once_flag Singleton<T>::initFlag;

    template <typename T>
    std::atomic<bool> Singleton<T>::alive { false };

}
