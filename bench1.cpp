#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void searchFile(const fs::path& directory, const std::string& fileName) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.path().filename() == fileName) {
            std::cout << "File found: " << entry.path() << std::endl;
            return;
        }
    }
    std::cout << "File not found." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <directory> <filename>" << std::endl;
        return 1;
    }
    searchFile(argv[1], argv[2]);
    return 0;
}
