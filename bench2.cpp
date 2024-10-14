#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <chrono>

struct Edge {
    int to, weight;
};

void dijkstra(int start, const std::vector<std::vector<Edge>>& graph) {
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

    std::cout << "Минимальное расстояние до всех вершин рассчитано\n";
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <количество вершин> <количество повторений>\n";
        return 1;
    }

    int vertex_count = std::stoi(argv[1]);
    int repeat_count = std::stoi(argv[2]);

    std::vector<std::vector<Edge>> graph(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
        for (int j = 0; j < 5; ++j) {
            int to = rand() % vertex_count;
            int weight = rand() % 100;
            graph[i].push_back({to, weight});
        }
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < repeat_count; ++i) {
        dijkstra(0, graph);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Общее время выполнения: " << elapsed.count() << " секунд" << std::endl;

    return 0;
}
