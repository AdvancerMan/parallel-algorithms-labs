#include <parlay/sequence.h>

#include "quick_sort_par.h"
#include "quick_sort_seq.h"

const int SEQ_SIZE = 1000;

void quick_sort_par_inplace(parlay::sequence<int> &sequence, size_t l, size_t r) {
    if (r - l <= SEQ_SIZE) {
        quick_sort_seq_inplace(sequence, l, r);
        return;
    }

    size_t partition_index = (l + r) / 2;
    int partition_value = sequence[partition_index];

    auto [left_part_r, right_part_l] = partition_seq_inplace(sequence, partition_value, l, r);
    
    parlay::par_do([&] { quick_sort_par_inplace(sequence, l, left_part_r); },
                   [&] { quick_sort_par_inplace(sequence, right_part_l, r); });
}

void quick_sort_par_inplace(parlay::sequence<int> &sequence) {
    quick_sort_par_inplace(sequence, 0, sequence.size());
}

