#ifndef GRAPH_PROCESSING_H
#define GRAPH_PROCESSING_H

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

struct Graph {
    std::vector<std::vector<int>> adjList;
    int numVertices;

    Graph(int n) : numVertices(n), adjList(n) {}

    void addEdge(int u, int v) {
        if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
            throw std::out_of_range("Vertex index out of range");
        }
        adjList[u].push_back(v);
        adjList[v].push_back(u); // For undirected graph
    }
};

void parallelBFS(const Graph& graph, int source, std::vector<int>& distances, std::mutex& mtx, std::condition_variable& cv);
void parallelDFS(const Graph& graph, int source, std::vector<int>& visited, std::mutex& mtx, std::condition_variable& cv);
std::vector<int> shortestPath(const Graph& graph, int source, int target);

#endif // GRAPH_PROCESSING_H
