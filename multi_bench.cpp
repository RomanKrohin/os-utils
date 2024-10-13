#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

void load_cpu(int id, int iterations) {
    std::cout << "Thread " << id << " started\n";
    volatile long long result = 0;
    for (int i = 0; i < iterations; ++i) {
        result += i * id;
    }
    std::cout << "Thread " << id << " finished\n";
}

int main() {
    int num_threads = std::thread::hardware_concurrency();
    int iterations = 100000000;
    
    std::vector<std::thread> threads;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(load_cpu, i, iterations));
    }
    
    for (auto& th : threads) {
        th.join();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Total execution time: " << elapsed.count() << " seconds\n";
    
    return 0;
}
