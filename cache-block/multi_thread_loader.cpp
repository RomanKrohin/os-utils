#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include "cache_block.h"

namespace fs = std::filesystem;

std::unordered_map<std::string, std::string> file_cache;
std::shared_mutex cache_mutex;

void cache_directory(const fs::path& dir) {
    std::unique_lock<std::shared_mutex> lock(cache_mutex);
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        file_cache[entry.path().filename().string()] = entry.path().string();
    }
}

bool search_file(const std::string& file_name) {
    std::shared_lock<std::shared_mutex> lock(cache_mutex);
    auto it = file_cache.find(file_name);
    if (it != file_cache.end()) {
        std::cout << "Файл найден: " << it->second << std::endl;
        return true;
    }
    return false;
}

void worker(const std::string& file_name, int repeat_count) {
    for (int i = 0; i < repeat_count; ++i) {
        if (!search_file(file_name)) {
            std::cout << "Файл не найден" << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Использование: " << argv[0] << " <имя_файла> <количество_повторений> <количество_потоков>\n";
        return 1;
    }

    std::string file_name = argv[1];
    int repeat_count = std::stoi(argv[2]);
    int thread_count = std::stoi(argv[3]);

    cache_directory(fs::current_path());

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(worker, file_name, repeat_count);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Общее время выполнения: " << elapsed.count() << " секунд" << std::endl;

    return 0;
}
