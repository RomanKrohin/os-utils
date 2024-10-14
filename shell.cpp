#define _GNU_SOURCE
#include <iostream>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <malloc.h>

#define STACK_SIZE (1024 * 1024)  // Размер стека больше не ограничен

// Функция для выполнения команд в дочернем процессе
int execute_command(void *args) {
    char **cmd_args = static_cast<char**>(args);
    execvp(cmd_args[0], cmd_args);  // Выполнение команды
    perror("execvp");               // Если execvp не сработает
    return 1;
}

int main() {
    std::string command;

    while (true) {
        std::cout << "shell> ";
        std::getline(std::cin, command);

        // Разделение команды на аргументы
        std::vector<char*> args;
        char *token = strtok(const_cast<char*>(command.c_str()), " ");
        while (token != nullptr) {
            args.push_back(token);
            token = strtok(nullptr, " ");
        }
        args.push_back(nullptr);

        if (args[0] != nullptr && strcmp(args[0], "exit") == 0) {
            break;
        }

        if (args[0] != nullptr && strcmp(args[0], "cd") == 0) {
            if (args[1] == nullptr) {
                std::cerr << "cd: ожидается аргумент\n";
            } else {
                if (chdir(args[1]) != 0) {
                    perror("cd");
                }
            }
            continue;
        }

        // Динамическое выделение стека
        void* child_stack = malloc(STACK_SIZE);  
        if (child_stack == nullptr) {
            perror("malloc");
            exit(1);
        }

        // Время начала выполнения команды
        auto start_time = std::chrono::high_resolution_clock::now();

        // Создаем новый процесс с помощью clone()
        pid_t pid = clone(execute_command, (char*)child_stack + STACK_SIZE, SIGCHLD, args.data());
        if (pid == -1) {
            perror("clone");
            exit(1);
        }

        // Ожидание завершения дочернего процесса
        wait(nullptr);

        // Освобождаем выделенный стек
        free(child_stack);

        // Время завершения команды
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;

        // Вывод времени выполнения
        std::cout << "Время выполнения: " << elapsed.count() << " секунд" << std::endl;
    }

    return 0;
}
