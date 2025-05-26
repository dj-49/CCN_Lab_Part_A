#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Distance Vector Routing (Bellman-Ford) algorithm
void dvr(const vector<vector<int>>& graph, int n, int src) {
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;

    // Relax all edges n-1 times
    for (int i = 0; i < n - 1; ++i) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (graph[u][v] != INT_MAX && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                }
            }
        }
    }

    // Check for negative-weight cycles
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (graph[u][v] != INT_MAX && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                cout << "Graph contains negative-weight cycle\n";
                return;
            }
        }
    }

    // Output distances
    cout << "Shortest distances from node " << src << ":\n";
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INT_MAX)
            cout << "Node " << i << ": Unreachable\n";
        else
            cout << "Node " << i << ": " << dist[i] << "\n";
    }
}

int main() {
    int n = 5; // Number of nodes
    // Adjacency matrix (INT_MAX for no edge)
    vector<vector<int>> graph = {
        {0, 10, INT_MAX, 5, INT_MAX},
        {10, 0, 5, 5, INT_MAX},
        {INT_MAX, 5, 0, INT_MAX, 10},
        {5, 5, INT_MAX, 0, 20},
        {INT_MAX, INT_MAX, 10, 20, 0}
    };

    int src = 0; // Source node
    dvr(graph, n, src);
    return 0;
}
