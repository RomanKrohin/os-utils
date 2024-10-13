#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>

struct Edge {
    int to, weight;
};

void shortestPath(int start, const std::vector<std::vector<Edge>>& graph) {
    std::vector<int> dist(graph.size(), INT_MAX);
    dist[start] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (const Edge& e : graph[u]) {
            if (dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
                pq.push({dist[e.to], e.to});
            }
        }
    }

    for (size_t i = 0; i < dist.size(); ++i) {
        std::cout << "Distance to node " << i << ": " << dist[i] << std::endl;
    }
}

int main() {
    std::vector<std::vector<Edge>> graph = {
        {{1, 4}, {2, 1}},
        {{2, 2}, {3, 5}},
        {{3, 1}},
        {}
    };

    shortestPath(0, graph);
    return 0;
}
