#include <chrono>
#include <iostream>
#include <string>
#include <tuple>

#include "bfs_par.h"
#include "bfs_seq.h"
#include "cubic_vertex.h"

double measure_time(std::string const& operation_name, std::function<void()> operation) {
    auto t_before = std::chrono::high_resolution_clock::now();
    operation();
    auto t_after = std::chrono::high_resolution_clock::now();
    
    double spent_time_seconds = std::chrono::duration_cast<std::chrono::microseconds>(t_after  - t_before).count() / 1000000.0;

    std::cout.precision(4);
    std::cout << std::fixed;
    std::cout << "Time for " << operation_name << " is " << spent_time_seconds << " s" << std::endl;

    return spent_time_seconds;
}

bool check_distances_are_correct(int* distances, size_t n) {
    size_t vertices_size = n + 1;
    for (size_t i = 0; i < vertices_size * vertices_size * vertices_size; i++) {
        size_t x = i % vertices_size;
        size_t y = i / vertices_size % vertices_size;
        size_t z = i / vertices_size / vertices_size % vertices_size;

        if (static_cast<size_t>(distances[i]) != x + y + z) {
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: bfs_benchmark <cube edge length> <measurements count>" << std::endl;
        std::cout << "e.g.: bfs_benchmark 500 5" << std::endl;
        return 1;
    }
  
    size_t n, k;
    try {
        n = std::stol(argv[1]);
        k = std::stol(argv[2]);
    } catch (...) {
        std::cout << "Usage: bfs_benchmark <cube edge length> <measurements count>" << std::endl;
        std::cout << "e.g.: bfs_benchmark 500 5" << std::endl;
        return 1;
    }
    
    double total_seq_seconds = 0.0;
    double total_par_seconds = 0.0;
    for (size_t i = 0; i < k; i++) {
        int* result_distances_par;
        total_par_seconds += measure_time("par", [&]() {
            CubicVertex start(n + 1, {0, 0, 0});
            result_distances_par = bfs_calculate_distances_par(start);
        });

        int* result_distances_seq;
        total_seq_seconds += measure_time("seq", [&]() {
            CubicVertex start(n + 1, {0, 0, 0});
            result_distances_seq = bfs_calculate_distances_seq(start);
        });

        std::cout << "Distance is correct: " << check_distances_are_correct(result_distances_par, n) << " " << check_distances_are_correct(result_distances_seq, n) << std::endl;
        std::cout << "Distance to {" << n << ", " << n << ", " << n << "} is " << result_distances_par[(n + 1) * (n + 1) * (n + 1) - 1] << std::endl;
    }

    std::cout.precision(4);
    std::cout << std::fixed;
    std::cout << "Average time for seq is " << total_seq_seconds / k << " s" << std::endl;
    std::cout << "Average time for par is " << total_par_seconds / k << " s" << std::endl;
    std::cout << "par is " << (total_seq_seconds / k) / (total_par_seconds / k) << " times faster than seq" << std::endl;
}

