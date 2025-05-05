#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
using namespace std;

class DVR {
private:
    int n;
    vector<vector<int>> g, d, next;
    const int INF = numeric_limits<int>::max() / 2;

public:
    DVR(int nodes) : n(nodes) {
        g.assign(n, vector<int>(n, INF));
        for (int i = 0; i < n; i++) g[i][i] = 0;
        d = g;
        next.assign(n, vector<int>(n, -1));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i == j || g[i][j] < INF) next[i][j] = j;
    }
    
    void addLink(int from, int to, int wt) {
        g[from][to] = d[from][to] = wt;
        next[from][to] = to;
    }
    
    void compute() {
        bool updated; int iter = 0;
        do {
            updated = false;
            cout << "\nIter " << ++iter << ":" << endl;
            
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
                                    cout << i << "→" << j << " via " << k << ": " << newDist << endl;
                                }
                            }
            
            if (updated) {
                cout << "Vectors:" << endl;
                for (int i = 0; i < n; i++) {
                    cout << i << ": ";
                    for (int j = 0; j < n; j++)
                        cout << (d[i][j] == INF ? "∞ " : to_string(d[i][j]) + " ");
                    cout << endl;
                }
            }
        } while (updated);
        cout << "Converged after " << iter << " iterations." << endl;
    }
    
    void showTable(int node) {
        cout << "\nRouting table for " << node << ":" << endl;
        cout << "Dest Dist Next" << endl;
        cout << "-------------" << endl;
        for (int j = 0; j < n; j++) {
            cout << setw(4) << j << " ";
            if (d[node][j] == INF) cout << setw(4) << "∞" << " N/A" << endl;
            else cout << setw(4) << d[node][j] << " " << setw(4) << next[node][j] << endl;
        }
    }
    
    void showPath(int src, int dst) {
        if (d[src][dst] == INF) {
            cout << "No path from " << src << " to " << dst << endl;
            return;
        }
        
        cout << "Path " << src << "→" << dst << ": ";
        vector<int> path;
        for (int curr = src; curr != dst; curr = next[curr][dst]) {
            path.push_back(curr);
            if (find(path.begin(), path.end()-1, curr) != path.end()-1) {
                cout << "Loop!" << endl;
                return;
            }
        }
        path.push_back(dst);
        
        for (size_t i = 0; i < path.size(); i++)
            cout << path[i] << (i < path.size()-1 ? "→" : "");
        cout << " (" << d[src][dst] << ")" << endl;
    }
    
    void showTopology() {
        cout << "\nNetwork:\n   ";
        for (int i = 0; i < n; i++) cout << setw(3) << i;
        cout << endl << string(3 + 3*n, '-') << endl;
        
        for (int i = 0; i < n; i++) {
            cout << setw(2) << i << "|";
            for (int j = 0; j < n; j++)
                cout << setw(3) << (g[i][j] == INF ? "∞" : to_string(g[i][j]));
            cout << endl;
        }
    }
    
    void linkFailure(int from, int to) {
        cout << "\nLink failure: " << from << "→" << to << endl;
        g[from][to] = INF;
        
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
    
    // Add bidirectional links
    net.addLink(0,1,1); net.addLink(1,0,1);
    net.addLink(0,3,7); net.addLink(3,0,7);
    net.addLink(1,2,2); net.addLink(2,1,2);
    net.addLink(1,3,4); net.addLink(3,1,4);
    net.addLink(2,3,2); net.addLink(3,2,2);
    net.addLink(2,4,5); net.addLink(4,2,5);
    net.addLink(3,4,1); net.addLink(4,3,1);
    
    cout << "=== DVR Simulation ===" << endl;
    net.showTopology();
    net.compute();
    
    cout << "\n=== Final Tables ===" << endl;
    for (int i = 0; i < 5; i++) net.showTable(i);
    
    cout << "\n=== Paths ===" << endl;
    net.showPath(0,4);
    net.showPath(4,0);
    
    net.linkFailure(3,4);
    
    cout << "\n=== Tables After Failure ===" << endl;
    for (int i = 0; i < 5; i++) net.showTable(i);
    
    cout << "\n=== Updated Paths ===" << endl;
    net.showPath(0,4);
    net.showPath(4,0);
    
    return 0;
}
