#include <iostream>
#include <cstring>
#include "cache_block.h"

int main() {
    int fd = lab2_open("example.txt", 4);
    if (fd == -1) {
        std::cerr << "Не удалось открыть файл\n";
        return 1;
    }
    std::cout << "Файл успешно открыт, дескриптор: " << fd << "\n";

    char read_buffer[100] = {0};
    ssize_t bytes_read = lab2_read(fd, read_buffer, sizeof(read_buffer) - 1);
    if (bytes_read == -1) {
        std::cerr << "Ошибка чтения файла\n";
        lab2_close(fd);
        return 1;
    }
    std::cout << "Прочитано " << bytes_read << " байт: " << read_buffer << "\n";

    // Перемещаем указатель в конец файла перед записью
    lab2_lseek(fd, 0, SEEK_END);

    const char *write_data = "a123123";
    ssize_t bytes_written = lab2_write(fd, write_data, strlen(write_data));
    if (bytes_written == -1) {
        std::cerr << "Ошибка записи файла\n";
        lab2_close(fd);
        return 1;
    }
    std::cout << "Записано " << bytes_written << " байт в файл\n";

    if (lab2_fsync(fd) == -1) {
        std::cerr << "Ошибка синхронизации с диском\n";
        lab2_close(fd);
        return 1;
    }
    std::cout << "Данные синхронизированы с диском\n";

    if (lab2_close(fd) == -1) {
        std::cerr << "Ошибка закрытия файла\n";
        return 1;
    }
    std::cout << "Файл закрыт\n";

    return 0;
}