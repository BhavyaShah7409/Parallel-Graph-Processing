#include <iostream>
#include "graph_processing.h"

int main() {
    // Create a sample graph
    Graph graph(6);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(2, 4);
    graph.addEdge(3, 5);
    graph.addEdge(4, 5);

    // Perform parallel BFS
    std::vector<int> distances(graph.numVertices, -1);
    std::mutex mtx;
    parallelBFS(graph, 0, distances, mtx);

    std::cout << "Distances from source (vertex 0) using BFS:" << std::endl;
    for (int i = 0; i < distances.size(); ++i) {
        std::cout << "Vertex " << i << ": " << distances[i] << std::endl;
    }

    // Perform parallel DFS
    std::vector<int> visited(graph.numVertices, 0);
    parallelDFS(graph, 0, visited, mtx);

    std::cout << "\nVisited vertices using DFS:" << std::endl;
    for (int i = 0; i < visited.size(); ++i) {
        if (visited[i])
            std::cout << "Vertex " << i << " was visited." << std::endl;
    }

    // Find shortest path
    try {
        std::vector<int> shortest = shortestPath(graph, 0, 5);
        std::cout << "\nShortest path from source (vertex 0) to target (vertex 5): ";
        for (int vertex : shortest) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
