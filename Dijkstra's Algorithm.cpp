#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

// Function to find shortest paths from source to all vertices
void dijkstra(const vector<vector<pair<int, int>>>& graph, int source, vector<int>& dist, vector<int>& parent) {
    int n = graph.size();
    dist.assign(n, numeric_limits<int>::max());
    parent.assign(n, -1);
    
    // Priority queue to store vertices that need to be processed
    // pair: (distance, vertex)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    // Distance from source to itself is 0
    dist[source] = 0;
    pq.push({0, source});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();
        
        // Skip if we've already found a better path
        if (d > dist[u])
            continue;
        
        // Process all neighbors of u
        for (const auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            
            // If we can find a shorter path to v through u
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

// Function to print the shortest path from source to destination
void printPath(const vector<int>& parent, int destination) {
    if (parent[destination] == -1) {
        cout << destination;
        return;
    }
    
    printPath(parent, parent[destination]);
    cout << " -> " << destination;
}

int main() {
    int n, m; // n = vertices, m = edges
    cout << "Enter the number of vertices and edges: ";
    cin >> n >> m;
    
    // Create adjacency list representation of the graph
    // graph[u] contains a list of pairs (v, weight) where v is adjacent to u with weight 'weight'
    vector<vector<pair<int, int>>> graph(n);
    
    cout << "Enter edges (u v weight): " << endl;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        // For undirected graph, uncomment the next line
        // graph[v].push_back({u, w});
    }
    
    int source;
    cout << "Enter source vertex: ";
    cin >> source;
    
    // Find shortest paths
    vector<int> dist, parent;
    dijkstra(graph, source, dist, parent);
    
    // Print results
    cout << "Shortest distances from source " << source << ":" << endl;
    for (int i = 0; i < n; i++) {
        cout << "To vertex " << i << ": ";
        if (dist[i] == numeric_limits<int>::max())
            cout << "No path exists" << endl;
        else {
            cout << "Distance = " << dist[i] << ", Path: ";
            printPath(parent, i);
            cout << endl;
        }
    }
    
    return 0;
}
