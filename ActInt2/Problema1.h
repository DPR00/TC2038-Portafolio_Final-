#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <climits>
#include <utility>

using namespace std;

typedef pair <int, int> iPair;

struct Graph{
    // V = cantidad de nodos (vertices)
    // E = cantidad de arcos (edges)
    int V, E;
    double costMSTKruskal;
    vector<pair<int, pair<int,int>>> edges; // Utilizar en Kruskal
    vector<vector<pair<int,double>>> adjList; // Se utiliza en Prim
    vector<pair<int, pair<int,int>>> selectEdgesK; // Los arcos de Kruskal

    Graph(int V, int E){
        this->V = V;
        this->E = E;
        adjList.resize(V);
        costMSTKruskal = 0;
    }

    // u = salida del arco
    // v = llegado del arco
    // w = costo del arco
    void addEdge(int u, int v, double w){
        edges.push_back({w, {u,v}}); // First = costo, second = conexion
        adjList[u].push_back({v, w});
    }

    void load(vector<string> &colNames, vector<string> &col1, vector<string> &col2, vector<int> &valEdges);
    void kruskalMST();
    void printResult(vector<string> &colNames);
};

// Disjoint Sets (Union-Find)
struct DisjointSets{
    int *parent, *rnk;
    int n;
    DisjointSets(int n){
        this->n = n;
        parent = new int[n+1];
        rnk = new int[n+1];
        for(int i=0; i<=n; i++){
            rnk[i] = 0;
            parent[i] = i;
        }
    }

    // Para encontrar el parent de 'u'
    int find(int u){
        if(u!=parent[u]){
            parent[u] = find(parent[u]);
        }
        return parent[u];
    }

    // Union por rank
    void merge(int x, int y){
        x = find(x);
        y = find(y);
        if(rnk[x] > rnk[y]){
            parent[y] = x;
        }
        else{
            parent[x] = y;
        }
        if(rnk[x] == rnk[y]){
            rnk[y]++;
        }
    }
};

void Graph::load(vector<string> &colNames, vector<string> &col1, vector<string> &col2, vector<int> &valEdges){

    int a, b;
    for(int i=0; i<E; i++){
        for(int j=0; j<V; j++){
            if(col1[i]==colNames[j]){
                a = j;
            }
            if(col2[i]==colNames[j]){
                b = j;
            }
        }
        addEdge(a, b, valEdges[i]);
        addEdge(b, a, valEdges[i]);
    }
}

// Complejidad: O(n*log2(n))
void Graph::kruskalMST(){
    costMSTKruskal = 0;
    sort(edges.begin(), edges.end());
    DisjointSets ds(V);
    for(auto it:edges){
        int u = it.second.first;
        int v = it.second.second;
        int set_u = ds.find(u);
        int set_v = ds.find(v);
        if(set_u != set_v){
            ds.merge(u, v);
            costMSTKruskal += it.first;
            selectEdgesK.push_back({it.first,{u,v}});
        }
    }
}

void Graph::printResult(vector<string> &colNames){
    int a, b;
    for(auto it:selectEdgesK){
        cout << colNames[it.second.first]<< " - " << colNames[it.second.second] << ": " << it.first << endl;
    }
    cout << endl << "Costo Total: " << costMSTKruskal << endl << endl;
}