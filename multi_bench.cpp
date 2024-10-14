#include <iostream>
#include <filesystem>
#include <string>
#include <chrono>
#include <vector>
#include <queue>
#include <limits>
#include <thread>

namespace fs = std::filesystem;

// Структура для графа
struct Edge {
    int to, weight;
};

// Функция для поиска файла
void search_file(const fs::path& dir, const std::string& file_name, int repeat_count) {
    for (int i = 0; i < repeat_count; ++i) {
        for (const auto& entry : fs::recursive_directory_iterator(dir)) {
            if (entry.path().filename() == file_name) {
                std::cout << "Файл найден: " << entry.path() << std::endl;
                return;
            }
        }
        std::cout << "Файл не найден" << std::endl;
    }
}

// Функция для выполнения алгоритма Дейкстры
void dijkstra(int start, const std::vector<std::vector<Edge>>& graph, int repeat_count) {
    for (int i = 0; i < repeat_count; ++i) {
        std::vector<int> dist(graph.size(), std::numeric_limits<int>::max());
        dist[start] = 0;

        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int v = pq.top().second;
            int current_dist = pq.top().first;
            pq.pop();

            if (current_dist > dist[v]) continue;

            for (const Edge& edge : graph[v]) {
                if (dist[v] + edge.weight < dist[edge.to]) {
                    dist[edge.to] = dist[v] + edge.weight;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
    }

    std::cout << "Алгоритм Дейкстры завершён\n";
}

// Основная программа
int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Использование: " << argv[0] << " <имя_файла> <количество вершин> <количество потоков>\n";
        return 1;
    }

    std::string file_name = argv[1];
    int vertex_count = std::stoi(argv[2]);
    int thread_count = std::stoi(argv[3]);

    // Подготовка графа для Дейкстры
    std::vector<std::vector<Edge>> graph(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
        for (int j = 0; j < 5; ++j) {
            int to = rand() % vertex_count;
            int weight = rand() % 100;
            graph[i].push_back({to, weight});
        }
    }

    // Начало замера времени
    auto start_time = std::chrono::high_resolution_clock::now();

    // Векторы потоков для двух задач
    std::vector<std::thread> threads;

    // Запуск потоков для поиска файлов
    for (int i = 0; i < thread_count / 2; ++i) {
        threads.emplace_back(search_file, fs::current_path(), file_name, 10);
    }

    // Запуск потоков для алгоритма Дейкстры
    for (int i = 0; i < thread_count / 2; ++i) {
        threads.emplace_back(dijkstra, 0, graph, 10);
    }

    // Ожидание завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    // Конец замера времени
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Общее время выполнения: " << elapsed.count() << " секунд" << std::endl;

    return 0;
}
