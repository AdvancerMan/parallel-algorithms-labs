#include <chrono>
#include <iostream>
#include <string>
#include <random>

#include <parlay/sequence.h>

#include "quick_sort_par.h"
#include "quick_sort_seq.h"

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

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
        std::cout << "Usage: quick_sort_benchmark <sort array size> <measurements count>" << std::endl;
        std::cout << "e.g.: quick_sort_benchmark 100000000 5" << std::endl;
        return 1;
    }
  
    size_t n, k;
    try {
        n = std::stol(argv[1]);
        k = std::stol(argv[2]);
    } catch (...) {
        std::cout << "Usage: quick_sort_benchmark <sort array size> <measurements count>" << std::endl;
        std::cout << "e.g.: quick_sort_benchmark 100000000 5" << std::endl;
        return 1;
    }
    
    double total_seq_seconds = 0.0;
    double total_par_seconds = 0.0;
    for (size_t i = 0; i < k; i++) {
        parlay::sequence<int> sequence(n);
        for (size_t i = 0; i < n; i++) {
            sequence[i] = rng();
        }
      
        parlay::sequence<int> sorted_par(sequence);
        total_par_seconds += measure_time("par", [&]() { quick_sort_par_inplace(sorted_par); });

        parlay::sequence<int> sorted_seq(sequence);
        total_seq_seconds += measure_time("seq", [&]() { quick_sort_seq_inplace(sorted_seq); });
        
        parlay::sequence<int> sorted_std(sequence);
        std::sort(sorted_std.begin(), sorted_std.end());
        
        std::cout << "Sorting is correct: " << (sorted_std == sorted_par) << " " << (sorted_std == sorted_seq) << std::endl;
    }

    std::cout.precision(4);
    std::cout << std::fixed;
    std::cout << "Average time for seq is " << total_seq_seconds / k << " s" << std::endl;
    std::cout << "Average time for par is " << total_par_seconds / k << " s" << std::endl;
    std::cout << "par is " << (total_seq_seconds / k) / (total_par_seconds / k) << " times faster than seq" << std::endl;
    
    {
        parlay::sequence<int> sequence(n);
        for (size_t i = 0; i < n; i++) {
            sequence[i] = rng();
        }
      
        std::cout << "O(log n) span algorithm is slow :(" << std::endl;

        parlay::sequence<int> sorted_par(sequence);
        measure_time("par", [&]() { quick_sort_par_inplace(sorted_par); });

        measure_time("par_theory_optimal", [&]() { quick_sort_par_theory_optimal(sequence); });
    }
}

