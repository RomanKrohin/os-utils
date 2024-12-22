#include <iostream>
#include <fstream>
#include <string>

int main() {
    const std::string proc_file = "/proc/my_lscpu";
    std::ifstream file(proc_file);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << proc_file << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();
    return 0;
}
