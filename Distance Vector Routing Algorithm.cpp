#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
using namespace std;

class DVR {
private:
    int n;
    vector<vector<int>> g, d, next;
    const int INF = numeric_limits<int>::max() / 2;

public:
    DVR(int nodes) : n(nodes) {
        g = d = vector<vector<int>>(n, vector<int>(n, INF));
        next.assign(n, vector<int>(n, -1));
        for (int i = 0; i < n; i++) {
            g[i][i] = d[i][i] = 0;
            next[i][i] = i;
        }
    }
    
    void addLink(int from, int to, int wt) {
        g[from][to] = d[from][to] = wt;
        next[from][to] = to;
    }
    
    void compute() {
        bool updated; int iter = 0;
        do {
            updated = false;
            cout << "Iter " << ++iter << endl;
            
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if (i != j)
                        for (int k = 0; k < n; k++)
                            if (g[i][k] != INF) {
                                int newDist = g[i][k] + d[k][j];
                                if (newDist < d[i][j]) {
                                    d[i][j] = newDist;
                                    next[i][j] = next[i][k];
                                    updated = true;
                                }
                            }
        } while (updated);
        cout << "Converged: " << iter << " iterations\n";
    }
    
    void showTable(int node) {
        cout << "Node " << node << " table:\n";
        for (int j = 0; j < n; j++) {
            if (j != node) {
                cout << "To " << j << ": ";
                if (d[node][j] == INF) cout << "∞\n";
                else cout << "dist=" << d[node][j] << " next=" << next[node][j] << endl;
            }
        }
    }
    
    void showPath(int src, int dst) {cout<<"Updated Path:";
        if (d[src][dst] == INF) {
            cout << "No path " << src << "→" << dst << endl;
            return;
        }
        
        cout << src << "→" << dst << ": ";
        for (int curr = src; curr != dst; curr = next[curr][dst])
            cout << curr << "→";
        cout << dst << " (" << d[src][dst] << ")\n";
    }
    
    void linkFailure(int from, int to) {
        cout << "Link down: " << from << "→" << to << endl;
        g[from][to] = INF;
        
        // Reset tables
        d = g;
        next.assign(n, vector<int>(n, -1));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i == j || g[i][j] < INF) next[i][j] = j;
        
        compute();
    }
};

int main() {
    DVR net(5);
    
    // Add links (bidirectional)
    int links[][3] = {{0,1,1}, {0,3,7}, {1,2,2}, {1,3,4}, {2,3,2}, {2,4,5}, {3,4,1}};
    for (auto& link : links) {
        net.addLink(link[0], link[1], link[2]);
        net.addLink(link[1], link[0], link[2]);
    }
    
    cout << "=== Initial State ===\n";
    net.compute();
    
    for (int i = 0; i < 5; i++) net.showTable(i);
    
    net.showPath(0,4);
    net.showPath(4,0);
    
    cout << "\n=== After Failure ===\n";
    net.linkFailure(3,4);
    
    for (int i = 0; i < 5; i++) net.showTable(i);
    
    net.showPath(0,4);
    net.showPath(4,0);
    
    return 0;
}
