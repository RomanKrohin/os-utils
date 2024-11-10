#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>

namespace fs = std::filesystem;

void search_file(const fs::path& dir, const std::string& file_name) {
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.path().filename() == file_name) {
            std::cout << "Файл найден: " << entry.path() << std::endl;
            return;
        }
    }
    std::cout << "Файл не найден" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <имя_файла> <количество_повторений>\n";
        return 1;
    }

    std::string file_name = argv[1];
    int repeat_count = std::stoi(argv[2]);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < repeat_count; ++i) {
        search_file(fs::current_path(), file_name);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Общее время выполнения: " << elapsed.count() << " секунд" << std::endl;

    return 0;
}
