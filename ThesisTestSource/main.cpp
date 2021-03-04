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

    int k = 4;

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

const string complete = "complete_graph";
const int complete_size = 499500;

const string super_dense = "super_dense_graph";
const int super_dense_size = 469879;

const string very_dense = "very_dense_graph";
const int very_dense_size = 445029;

const string dense = "dense_graph";
const int dense_size = 378036;

const string semi_dense = "semi_dense_graph";
const int semi_dense_size = 312846;

const string sparse = "sparse_graph";
const int sparse_size = 117419;

const string very_sparse = "very_sparse_graph";
const int very_sparse_size = 14878;



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

vector<Edge> read_generated_graph_file(const string& file_name, int size){
    vector<Edge> edges(size*2);
    int i = 0;

    int u,v; double weight;
    ifstream file(file_name);
    if (file.is_open()){
        while (file >> u >> v >> weight){
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
    double stretch;
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

void density_test_wulff(){

    vector<string> files = {
            complete,
            super_dense,
            very_dense,
            dense,
            semi_dense,
            sparse,
            very_sparse
    };

    vector<int> sizes ={
            complete_size,
            super_dense_size,
            very_dense_size,
            dense_size,
            semi_dense_size,
            sparse_size,
            very_sparse_size
    };

    int n = 1000;

    vector<int> k_values = {
            2,
            3,
            5,
            8,
            10,
            13
    };

    const string oracle = "wulff_";
    //const string file = very_sparse;
    //int _size = very_sparse_size;
    //int k = 13;

    for (int i = 0; i < files.size(); i++) {
        string file = files[i];
        int size_ = sizes[i];
        for (int k_ = 0; k_ < k_values.size(); k_++){
            int k = k_values[k_];
            log("Loading file.");
            auto edges = read_generated_graph_file("../" + file, size_);
            log("File loaded.");

            try {
                auto t1 = chrono::high_resolution_clock::now();
                WulffNilsenGeneralADO ado(k, n, edges);
                auto t2 = chrono::high_resolution_clock::now();

                auto prepro_time = get_miliseconds(t1, t2);

                ado.calculate_size();

                int size = ado.GetSize();

                log("\n\nResults:");
                log("preprocessing time: " + to_string(prepro_time) + "ms");
                log("size in n: " + to_string(size));

                int z = 20.0;
                int y_size = 100;
                vector<int> xs = {318, 331, 417, 634, 516, 497, 896, 126, 672, 151};
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
                        query_results.push_back({dist/exact_distances[t][y], time/z});
                    }
                    t++;
                }

                ofstream result_file(oracle + file + "_" + to_string(k) + ".txt", std::ios_base::app);
                for (auto r : query_results)
                    result_file << to_string(r.stretch) + "," + to_string(r.time) << "," << k << "\n";
                result_file.close();
            }catch (std::invalid_argument& e){
                k_--;
            }
        }
    }
}


void density_test_thorup(){

    vector<string> files = {
            complete,
            super_dense,
            very_dense,
            dense,
            semi_dense,
            sparse,
            very_sparse
    };

    vector<int> sizes ={
            complete_size,
            super_dense_size,
            very_dense_size,
            dense_size,
            semi_dense_size,
            sparse_size,
            very_sparse_size
    };

    int n = 1000;

    vector<int> k_values = {
            2,
            3,
            5,
            8,
            10,
            13
    };

    const string oracle = "thorup_";
    //const string file = very_sparse;
    //int _size = very_sparse_size;
    //int k = 13;

    for (int i = 0; i < files.size(); i++) {
        string file = files[i];
        int size_ = sizes[i];
        for (int k_ = 0; k_ < k_values.size(); k_++){
            int k = k_values[k_];
            log("Loading file.");
            auto edges = read_generated_graph_file("../" + file, size_);
            log("File loaded.");

            try {
                auto t1 = chrono::high_resolution_clock::now();
                ThorupZwickGeneralADO ado(k, n, edges);
                auto t2 = chrono::high_resolution_clock::now();

                auto prepro_time = get_miliseconds(t1, t2);

                ado.calculate_size();

                int size = ado.GetSize();

                log("\n\nResults:");
                log("preprocessing time: " + to_string(prepro_time) + "ms");
                log("size in n: " + to_string(size));

                int z = 20.0;
                int y_size = 100;
                vector<int> xs = {318, 331, 417, 634, 516, 497, 896, 126, 672, 151};
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
                        query_results.push_back({dist/exact_distances[t][y], time/z});
                    }
                    t++;
                }

                ofstream result_file(oracle + file + "_" + to_string(k) + ".txt", std::ios_base::app);
                for (auto r : query_results)
                    result_file << to_string(r.stretch) + "," + to_string(r.time) << "," << k << "\n";
                result_file.close();
            } catch (std::invalid_argument& e){
                k_--;
            }
        }
    }
}

void real_world_test(){
    const string oracle = "wulff_";
    const string out_file = "real_world_";
    int n = 264346;
    int e = 733846;

    vector<int> k_values = {
            //2,
            3,
            5,
            8,
            10,
            13,
            15,
            18
    };

    for (int i = 0; i < k_values.size(); i++){
        int k = k_values[i];
        try {
            log("Loading file.");
            auto edges = read_graph_file(e);
            log("File loaded.");

            auto t1 = chrono::high_resolution_clock::now();
            WulffNilsenGeneralADO ado(k, n, edges);
            auto t2 = chrono::high_resolution_clock::now();

            auto prepro_time = get_miliseconds(t1, t2);

            ado.calculate_size();

            int size = ado.GetSize();

            log("\n\nResults:");
            log("preprocessing time: " + to_string(prepro_time) + "ms");
            log("size in n: " + to_string(size));

            int z = 20.0;
            int y_size = 1000;
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
                    query_results.push_back({dist/exact_distances[t][y], time/z});
                }
                t++;
            }

            ofstream result_file(oracle + out_file + to_string(k) + ".txt", std::ios_base::app);
            for (auto r : query_results)
                result_file << to_string(r.stretch) + "," + to_string(r.time) << "," << k << "\n";
            result_file.close();
        }catch (std::invalid_argument& e){
            i--;
        }
    }
}

int main() {

    //MinorTest();
    for (int i = 0; i < 10; i++) {
        density_test_wulff();
        density_test_thorup();
    }
    //for (int i = 0; i < 3; ++i) {
    //    log("\n Iteration:" + to_string(i) + "\n");
    //    real_world_test();
    //}

    return 0;
}
