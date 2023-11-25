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
        size_t result_distance_par;
        total_par_seconds += measure_time("par", [&]() {
            CubicVertex start(n + 1, {0, 0, 0});
            result_distance_par = bfs_calculate_distance_par(start, {n, n, n});
        });

        size_t result_distance_seq;
        total_seq_seconds += measure_time("seq", [&]() {
            CubicVertex start(n + 1, {0, 0, 0});
            result_distance_seq = bfs_calculate_distance_seq(start, {n, n, n});
        });
        
        std::cout << "Distance is correct: " << (3 * n == result_distance_par) << " " << (3 * n == result_distance_seq) << std::endl;
    }

    std::cout.precision(4);
    std::cout << std::fixed;
    std::cout << "Average time for seq is " << total_seq_seconds / k << " s" << std::endl;
    std::cout << "Average time for par is " << total_par_seconds / k << " s" << std::endl;
    std::cout << "par is " << (total_seq_seconds / k) / (total_par_seconds / k) << " times faster than seq" << std::endl;
}

