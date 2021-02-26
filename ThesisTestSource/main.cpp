#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <thread>
#include <chrono>
#include <omp.h>

#include "ApproximationDistanceOracle/ThorupZwickGeneralADO.hpp"
#include "ApproximationDistanceOracle/WulffNilsenGeneralADO.hpp"
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

    WulffNilsenGeneralADO ado(k, 20, edges);

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            try{
                ado.GetDistance(i,j);
            }
            catch (std::invalid_argument& s){
                log(to_string(i) + "," + to_string(j));
            }
        }
    }

    cout << "All done!!!!" << endl;
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

vector<Edge> read_generated_graph_file(string &file_name, int size){
    vector<Edge> edges(size*2);
    int i = 0;

    int u,v; double weight;
    char a;
    ifstream file(file_name);
    if (file.is_open()){
        while (file >> a >> u >> v >> weight){
            edges[i] = {u, v, weight};
            i++;
            edges[i] = {v, u, weight};
            i++;
        }
        file.close();
    }
    else{
        throw "File not opened!";
    }

    return edges;
}

unsigned int get_miliseconds(std::chrono::time_point<std::chrono::system_clock> t1, std::chrono::time_point<std::chrono::system_clock> t2){
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

unsigned int get_microseconds(std::chrono::time_point<std::chrono::system_clock> t1, std::chrono::time_point<std::chrono::system_clock> t2){
    return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
}

unsigned int get_nanoseconds(std::chrono::time_point<std::chrono::system_clock> t1, std::chrono::time_point<std::chrono::system_clock> t2){
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
}

vector<int> build_ys(int size, int max, vector<int> &xs){
    unordered_set<int> illigal_indeces;
    for (auto x : xs)
        illigal_indeces.insert(x);

    std::random_device rd;
    std::mt19937 gen (rd());
    std::uniform_int_distribution<int> dis (0, max - 1);
    vector<int> ys(size);
    for (int i = 0; i < size; i++) {
        ys[i] = dis(gen);
        if (illigal_indeces.contains(ys[i]))
            i--;
    }
    return ys;
}

struct result{
    double expected, actual;
    double time;
};

struct section{
    AdjecencyMatrix &graph;
    int start, end, size;
};

void run_section(section s){
    for (int i = s.start; i < s.end; i++) {
        shortest_distances(i, s.graph, s.size);
        log(to_string(i));
    }
}

int main() {
    int cont;

    const string oracle = "wulff_";
    const string out_file = "real_world_";
    int k = 18;
    int n = 264346;
    int e = 733846;

    //MinorTest();

    //return 0;

    log("Loading file.");
    auto edges = read_graph_file(e);
    log("File loaded.");

    auto t1 = chrono::high_resolution_clock::now();
    WulffNilsenGeneralADO ado(k, n, edges);
    auto t2 = chrono::high_resolution_clock::now();

    auto prepro_time = get_miliseconds(t1, t2);

    cin >> cont;
    ado.calculate_size();

    int size = ado.GetSize();

    log("\n\nResults:");
    log("preprocessing time: " + to_string(prepro_time) + "ms");
    log("size in n: " + to_string(size));

    int z = 20.0;
    int y_size = 100;
    vector<int> xs = {235725, 136521, 212727, 260649, 51984, 160358, 121788, 83423, 226130, 129519};
    vector<int> ys = build_ys(y_size, n, xs);

    vector<vector<double>> exact_distances(xs.size());
    vector<result> query_results;

    AdjecencyMatrix graph(n,edges);
    for (int i = 0; i < xs.size(); i++) {
        exact_distances[i] = shortest_distances(xs[i], graph, n);
    }

    int t = 0;
    for (auto x : xs){
        for (auto y: ys){
            double time = 0;
            double dist = 0;
            for (int i = 0; i < z; i++) {
                auto t_start = chrono::high_resolution_clock::now();
                auto r = ado.GetDistance(x, y);
                auto t_end = chrono::high_resolution_clock::now();
                time += get_nanoseconds(t_start, t_end);
                if (i == 0)
                    dist = r;
            }
            query_results.push_back({exact_distances[t][y], dist, time/z});
        }
        t++;
    }

    ofstream result_file(oracle + out_file + to_string(k) + ".txt");
    for (auto r : query_results)
        result_file << to_string(r.expected) + "," + to_string(r.actual) + "," + to_string(r.time) << endl;
    result_file.close();

    return 0;
}
