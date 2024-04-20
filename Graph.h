#include <iostream>
#include <vector>
#include <unordered_map>
#include "Match.h"
using namespace std;

class Graph {
private:
    unordered_map<int, vector<int>> adj_list;

public:
    void add_vertex(int vertex) {
        if (adj_list.find(vertex) == adj_list.end()) {
            adj_list[vertex] = vector<int>();
        }
    }

    void add_edge(int vertex1, int vertex2) {
        add_vertex(vertex1);
        add_vertex(vertex2);
        adj_list[vertex1].push_back(vertex2);
        // Uncomment the following line for an undirected graph
    }

    vector<int> get_neighbors(int vertex) {
        if (adj_list.find(vertex) != adj_list.end()) {
            return adj_list[vertex];
        } else {
            return vector<int>();
        }
    }

    void print() {
        for (const auto& pair : adj_list) {
            cout << pair.first << " -> ";
            for (int neighbor : pair.second) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }
};