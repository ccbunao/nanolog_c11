#include "NanoLog.hpp"
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <cstdio>
#include <iostream>
using namespace std;

/* Returns microseconds since epoch */
uint64_t timestamp_now()
{
    return std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::microseconds(1);
}

void nanolog_benchmark()
{
    int const iterations = 100000;
    char const *const benchmark = "benchmark";
    uint64_t begin = timestamp_now();
    for (int i = 0; i < iterations; ++i)
        LOG_INFO << "Logging " << benchmark << i << 0 << 'K' << -42.42;
    uint64_t end = timestamp_now();
    long int avg_latency = (end - begin) * 1000 / iterations;
    printf("\tAverage NanoLog Latency = %ld nanoseconds\n", avg_latency);
}

template <typename Function>
// 定义一个函数run_benchmark，该函数接受一个可调用对象f和一个整数thread_count作为参数
void run_benchmark(Function &&f, int thread_count)
{
    // 打印线程数量
    printf("Thread count: %d", thread_count);

    // 创建一个std::vector容器，用于存储std::thread对象
    std::vector<std::thread> threads;

    // 使用for循环创建指定数量的线程，并将这些线程添加到threads容器中
    for (int i = 0; i < thread_count; ++i)
    {
        // 将可调用对象f添加到线程容器中
        threads.emplace_back(f);
    }

    // 使用for循环等待所有线程完成执行
    for (int i = 0; i < thread_count; ++i)
    {
        // 调用每个线程对象的join方法，等待其完成执行
        threads[i].join();
    }
}

int main()
{
    // Ring buffer size is passed as 10 mega bytes.
    // Since each log line = 256 bytes, thats 40960 slots.
    nanolog::initialize(nanolog::NonGuaranteedLogger(10), "/tmp/", "nanolog", 1);
    for (auto threads : {1, 2, 3, 4, 5})
        run_benchmark(nanolog_benchmark, threads);

    return 0;
}
