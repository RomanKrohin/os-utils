#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <vector>
#include <deque>
#include <sys/types.h>
#include <sys/stat.h>
#include <unordered_map>
#include "cache_block.h"

namespace fs = std::filesystem;

std::unordered_map<std::string, std::string> file_cache;

void cache_directory(const fs::path& dir) {
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        file_cache[entry.path().filename().string()] = entry.path().string();
    }
}

bool search_file(const std::string& file_name) {
    auto it = file_cache.find(file_name);
    if (it != file_cache.end()) {
        std::cout << "Файл найден: " << it->second << std::endl;
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <имя_файла> <количество_повторений>\n";
        return 1;
    }

    std::string file_name = argv[1];
    int repeat_count = std::stoi(argv[2]);

    // Кэшируем содержимое директории один раз
    cache_directory(fs::current_path());

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < repeat_count; ++i) {
        if (!search_file(file_name)) {
            std::cout << "Файл не найден" << std::endl;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Общее время выполнения: " << elapsed.count() << " секунд" << std::endl;

    return 0;
}