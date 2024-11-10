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

#define STACK_SIZE (1024 * 1024) 

int execute_command(void *args) {
    char **cmd_args = static_cast<char**>(args);
    execvp(cmd_args[0], cmd_args);
    perror("execvp");
    return 1;
}

int main() {
    std::string command;

    while (true) {
        std::cout << "shell> ";
        std::getline(std::cin, command);

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

        void* child_stack = malloc(STACK_SIZE);  
        if (child_stack == nullptr) {
            perror("malloc");
            exit(1);
        }

        auto start_time = std::chrono::high_resolution_clock::now();

        pid_t pid = clone(execute_command, (char*)child_stack + STACK_SIZE, SIGCHLD, args.data());
        if (pid == -1) {
            perror("clone");
            exit(1);
        }

        wait(nullptr);

        free(child_stack);

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;

        std::cout << "Время выполнения: " << elapsed.count() << " секунд" << std::endl;
    }

    return 0;
}
