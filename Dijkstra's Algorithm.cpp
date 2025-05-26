#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Structure to represent a node in the priority queue
struct Node {
    int vertex, dist;
    bool operator>(const Node& other) const { return dist > other.dist; }
};

// Dijkstra's algorithm for shortest path from source to all vertices
vector<int> dijkstra(const vector<vector<int>>& graph, int src, int n) {
    vector<int> dist(n, INT_MAX);
    vector<bool> visited(n, false);
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    dist[src] = 0;
    pq.push({src, 0});

    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();
        int u = curr.vertex;

        if (visited[u]) continue;
        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph[u][v] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                pq.push({v, dist[v]});
            }
        }
    }
    return dist;
}

// Main DVR function to compute routes
void dvr(const vector<vector<int>>& graph, int n, int start) {
    cout << "Shortest distances from node " << start << ":\n";
    vector<int> distances = dijkstra(graph, start, n);

    for (int i = 0; i < n; ++i) {
        if (distances[i] == INT_MAX)
            cout << "Node " << i << ": Unreachable\n";
        else
            cout << "Node " << i << ": " << distances[i] << "\n";
    }
}

int main() {
    int n = 5; // Number of nodes
    // Adjacency matrix (INT_MAX for no edge)
    vector<vector<int>> graph = {
        {0, 1, INT_MAX, 5, INT_MAX},
        {10, 0, 5, 5, INT_MAX},
        {INT_MAX, 5, 0, INT_MAX, 10},
        {5, 5, INT_MAX, 0, 20},
        {INT_MAX, INT_MAX, 10, 20, 0}
    };

    int start = 0; // Starting node
    dvr(graph, n, start);
    return 0;
}
