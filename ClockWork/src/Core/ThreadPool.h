#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>

namespace CW {

    class ThreadPool 
    {
    public:
        using Task = std::function<void(size_t)>;

        ThreadPool(size_t thread_count = 0) 
        {
            Start(thread_count); 
        }

        ~ThreadPool() 
        {
            Wait();
            Stop();
            Join();
        }

        inline void Push(Task&& fun);
        inline void Wait();

        size_t GetThreadCount() const { return _threads.size(); }

    private:
        static inline void Worker(ThreadPool*, size_t);

        inline void Start(size_t);
        inline void Stop();
        inline void Join();

        int _busyCount = 0;
        bool _shouldStop = false;
        
        std::mutex _mutex;
        std::vector<std::thread> _threads;
        std::condition_variable _available;
        std::condition_variable _completed;
        std::queue<Task> _tasks;
    };

    void ThreadPool::Push(Task&& task) 
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _tasks.emplace(std::move(task));
        }
        _available.notify_one();
    }

    void ThreadPool::Wait() 
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _completed.wait(lock, [this] { return _busyCount == 0 && _tasks.empty(); });
    }

    void ThreadPool::Worker(ThreadPool* pool, size_t thread_id) 
    {
        while (true) 
        {
            Task task;
            {
                std::unique_lock<std::mutex> lock(pool->_mutex);
                pool->_available.wait(lock, [pool] { return pool->_shouldStop || !pool->_tasks.empty(); });
                if (pool->_shouldStop && pool->_tasks.empty())
                    break;
                task = std::move(pool->_tasks.front());
                pool->_tasks.pop();
                pool->_busyCount++;
            }
            task(thread_id);
            {
                std::unique_lock<std::mutex> lock(pool->_mutex);
                pool->_busyCount--;
            }
            pool->_completed.notify_one();
        }
    }

    void ThreadPool::Start(size_t thread_count) 
    {
        if (thread_count == 0)
            thread_count = std::max(1u, std::thread::hardware_concurrency());
        for (size_t i = 0; i < thread_count; ++i)
            _threads.emplace_back(Worker, this, i);
    }

    void ThreadPool::Stop() 
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _shouldStop = true;
        }
        _available.notify_all();
    }

    void ThreadPool::Join() 
    {
        for (auto& thread : _threads)
            thread.join();
    }
} 