#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stack>
#include <thread>
#include <chrono>
#include <omp.h>

#include "ApproximationDistanceOracle/ThorupZwickGeneralADO.hpp"
//#include "ApproximationDistanceOracle/ChechikGeneralADO.hpp"
#include "HelperTypes/Logger.hpp"
#include "HelperTypes/SortedDistanceMatrix.hpp"

using namespace std;

void MinorTest(){
    vector<Edge> edges = {
            {0, 4, 2},
            {4, 0, 2},
            {0, 3, 4},
            {3, 0, 4},
            {3, 8, 9},
            {8, 3, 9},
            {3, 12, 1},
            {12, 3, 1},
            {4, 1, 4},
            {1, 4, 4},
            {4, 9, 3},
            {9, 4, 3},
            {1, 2, 1},
            {1, 5, 3},
            {2, 1, 1},
            {2, 5, 7},
            {2, 6, 2},
            {8, 9, 4},
            {9, 8, 4},
            {9, 5, 6},
            {9, 8, 3},
            {5, 1, 3},
            {5, 2, 7},
            {5, 6, 4},
            {5, 14, 1},
            {5, 9, 6},
            {6, 2, 2},
            {6, 5, 4},
            {6, 10, 8},
            {6, 11, 3},
            {7, 12, 8},
            {7, 17, 3},
            {12, 7, 8},
            {12, 13, 4},
            {13, 9, 1},
            {13, 12, 4},
            {13, 14, 4},
            {13, 17, 5},
            {13, 19, 7},
            {14, 5, 1},
            {14, 13, 4},
            {14, 18, 3},
            {10, 6, 8},
            {10, 11, 4},
            {10, 15, 2},
            {11, 10, 4},
            {11, 6, 3},
            {11, 16, 10},
            {17, 7, 3},
            {17, 13, 5},
            {17, 19, 6},
            {18, 14, 3},
            {18, 15, 5},
            {15, 10, 2},
            {15, 18, 5},
            {15, 16, 3},
            {16, 19, 3},
            {16, 15, 3},
            {16, 11, 10},
            {19, 13, 7},
            {19, 16, 3},
            {19, 17, 6},
    };

    int k = 2;

    ThorupZwickGeneralADO ado(k, 20, edges);

    double dist1 = ado.GetDistance(0, 16);
    double dist2 = ado.GetDistance(16, 0);

    cout << "All done!!!!" << endl;
    cout << dist1 << endl;
    cout << dist2 << endl;
    cout << "" << endl;
}

vector<Edge> read_graph_file(int size){
    vector<Edge> edges(size);
    int i = 0;

    int u,v; double weight;
    char a;
    ifstream file("../USA-road-d.NY.gr");
    if (file.is_open()){
        while (file >> a >> u >> v >> weight){
            edges[i] = {u - 1, v - 1, weight};
            i++;
        }
        file.close();
    }
    else{
        throw "File not opened!";
    }

    return edges;
}
struct t_arg{
    int size;
    AdjecencyMatrix graph;
    int beginning, end;
};

class section_container{
public:
    int size;
    AdjecencyMatrix graph;
    int beginning;
    int end;

    section_container(AdjecencyMatrix graph, int b, int e) :
    graph(graph),
    size(graph.length()),
    beginning(b),
    end(e){

    }
};

void shortest_distances_2(int s, AdjecencyMatrix &graph, int v_size){
    MinHeap dpq (v_size);
    dpq.insertKey(s, 0);
    for (int j = 0; j < v_size; j++) {
        if (j != s)
            dpq.insertKey(j, INFINITY);
    }

    vector<double> d_temp(v_size, INFINITY);
    d_temp[s] = 0;
    vector<bool> visited_vertices(v_size);

    QueueItem q = dpq.extractMin();
    while (q.v != -1){
        if (!visited_vertices[q.v]){
            auto adjecent_vertices = graph[q.v];
            for (auto edge: adjecent_vertices){
                double tmp_dist = d_temp[q.v] + edge.weight;
                if (tmp_dist < d_temp[edge.v]){
                    d_temp[edge.v] = tmp_dist;
                    dpq.decreaseKey(edge.v, d_temp[edge.v]);
                }
            }
            visited_vertices[q.v] = true;
        }
        q = dpq.extractMin();
    }
}

int main() {

    int k = 10;
    int n = 264346;
    int e = 733846;

    //MinorTest();

    log("Loading file.");
    auto edges = read_graph_file(e);
    log("File loaded.");
    ThorupZwickGeneralADO ado(k, n, edges);

    return 0;
}
