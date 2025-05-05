#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

void dijkstra(const vector<vector<pair<int, int>>>& g, int src, vector<int>& dist, vector<int>& parent) {
    dist.assign(g.size(), numeric_limits<int>::max());
    parent.assign(g.size(), -1);
    dist[src] = 0;
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        
        if (d > dist[u]) continue;
        
        for (auto [v, w] : g[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

void printPath(const vector<int>& parent, int dest) {
    if (parent[dest] == -1) { cout << dest; return; }
    printPath(parent, parent[dest]);
    cout << " -> " << dest;
}

int main() {
    int n, m;
    cout << "Enter the number of vertices and edges: ";
    cin >> n >> m;
    
    vector<vector<pair<int, int>>> graph(n);
    
    cout << "Enter edges (u v weight):" << endl;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        // For undirected graph: graph[v].push_back({u, w});
    }
    
    int source;
    cout << "Enter source vertex: ";
    cin >> source;
    
    vector<int> dist, parent;
    dijkstra(graph, source, dist, parent);
    
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
