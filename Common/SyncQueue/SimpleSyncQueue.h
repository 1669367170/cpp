#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

// unique_lock 与 lock_guard
// condition_variable与condition_variable_any ？
// recursive_mutex

template<typename T>
class SimpleSyncQueue {
public:
    SimpleSyncQueue(int32_t maxSize) : maxSize(maxSize) {};

    void Producer(const T& elem)
    {
        // std::lock_guard<std::mutex> lock(mtx);
        // while (IsFull()) {
        //     notFull.wait(mtx); // 生产者等待"缓冲区不为满"这一条件发生.
        // }
        std::unique_lock<std::mutex> lock(mtx);
        while (syncQueue.size() >= maxSize) { // 使用IsFull不行
            notFull.wait(lock); // 生产者等待"缓冲区不为满"这一条件发生.
        }
        // std::cout << "Producer one elem: " << elem << " , size: " << this->Size() << std::endl; // 不行, 递归锁？
        // std::cout << "Producer one elem: " << elem << " , size: " << syncQueue.size() << std::endl;
        syncQueue.push(elem);
        notEmpty.notify_one();
    }

    const T& Consumer()
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (syncQueue.size() <= 0) {
            notEmpty.wait(lock); // 消费者等待"缓冲区不为空"这一条件发生.
        }
        const T& elem = syncQueue.front();
        // std::cout << "Consumer one elem: " << elem << " , size: " << syncQueue.size() << std::endl;
        syncQueue.pop();
        notFull.notify_one();
        return elem;
    }

    bool IsEmpty()
    {
        std::unique_lock<std::mutex> lock(mtx);
        return syncQueue.size() <= 0;
    }

    bool IsFull()
    {
        std::unique_lock<std::mutex> lock(mtx);
        return syncQueue.size() >= maxSize;
    }

    size_t Size()
    {
        std::unique_lock<std::mutex> lock(mtx);
        return syncQueue.size();
    }

private:
    int32_t maxSize{ 0 };
    std::queue<T> syncQueue;
    std::mutex mtx;
    std::condition_variable notFull;
    std::condition_variable notEmpty;
};

void SimpleSyncQueueTest()
{
    std::cout << "SimpleSyncQueueTest" << std::endl;
    SimpleSyncQueue<int> queue(9);

    std::mutex logMtx;
    // 生产者线程
    std::thread producer([&queue, &logMtx] {
        for (int i = 0; i < 10; ++i) {
            queue.Producer(i);
            {
                std::unique_lock<std::mutex>(logMtx);
                // TODO 日志打印会异步
                std::cout << "Producer elem [" << i << "], size: " << queue.Size() - 1 << " -> " << queue.Size() << std::endl;
            }
        }
        });

    // 消费者线程
    std::thread consumer([&queue, &logMtx] {
        for (int i = 0; i < 10; ++i) {
            int value = queue.Consumer();
            {
                std::unique_lock<std::mutex>(logMtx);
                std::cout << "Consumer elem [" << value << "], size: " << queue.Size() + 1 << " -> " << queue.Size() << std::endl;
            }
        }
        });

    producer.join();
    consumer.join();
}