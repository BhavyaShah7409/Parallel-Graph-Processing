#include "graph_processing.h"

void parallelBFS(const Graph& graph, int source, std::vector<int>& distances, std::mutex& mtx) {
    std::queue<int> q;
    q.push(source);
    distances[source] = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph.adjList[u]) {
            std::unique_lock<std::mutex> lock(mtx);
            if (distances[v] == -1) {
                distances[v] = distances[u] + 1;
                q.push(v);
            }
        }
    }
}

void parallelDFS(const Graph& graph, int source, std::vector<int>& visited, std::mutex& mtx) {
    visited[source] = 1;

    for (int v : graph.adjList[source]) {
        std::unique_lock<std::mutex> lock(mtx);
        if (!visited[v]) {
            parallelDFS(graph, v, visited, mtx);
        }
    }
}

std::vector<int> shortestPath(const Graph& graph, int source, int target) {
    if (source < 0 || source >= graph.numVertices || target < 0 || target >= graph.numVertices) {
        throw std::out_of_range("Vertex index out of range");
    }

    std::vector<int> distances(graph.numVertices, -1);
    std::mutex mtx;

    // Perform parallel BFS
    parallelBFS(graph, source, distances, mtx);

    // Check if the target vertex is reachable
    if (distances[target] == -1) {
        return {}; // Target vertex is unreachable
    }

    // Perform parallel DFS to find the shortest path
    std::vector<int> visited(graph.numVertices, 0);
    parallelDFS(graph, source, visited, mtx);

    // Reconstruct the shortest path
    std::vector<int> path;
    int curr = target;
    while (curr != source) {
        path.push_back(curr);
        for (int v : graph.adjList[curr]) {
            if (distances[v] == distances[curr] - 1) {
                curr = v;
                break;
            }
        }
    }
    path.push_back(source);
    std::reverse(path.begin(), path.end());
    return path;
}
