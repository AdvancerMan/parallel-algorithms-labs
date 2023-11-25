#include <parlay/sequence.h>

#include "quick_sort_seq.h"

std::pair<size_t, size_t> partition_seq_inplace(parlay::sequence<int> &sequence, int partition_value, size_t l, size_t r) {
    size_t left_part_r = l;
    for (size_t i = l; i < r; i++) {
        if (sequence[i] < partition_value) {
            std::swap(sequence[left_part_r], sequence[i]);
            left_part_r++;
        }
    }
    
    size_t right_part_l = left_part_r;
    for (size_t i = left_part_r; i < r; i++) {
        if (sequence[i] == partition_value) {
            std::swap(sequence[right_part_l], sequence[i]);
            right_part_l++;
        }
    }
    
    return {left_part_r, right_part_l};
}

std::pair<size_t, size_t> partition_seq_inplace(parlay::sequence<int> &sequence, int partition_value) {
    return partition_seq_inplace(sequence, partition_value, 0, sequence.size());
}

void quick_sort_seq_inplace(parlay::sequence<int> &sequence, size_t l, size_t r) {
    if (l + 1 >= r) {
        return;
    }

    size_t partition_index = (l + r) / 2;
    int partition_value = sequence[partition_index];
    auto [left_part_r, right_part_l] = partition_seq_inplace(sequence, partition_value, l, r);
    
    quick_sort_seq_inplace(sequence, l, left_part_r);
    quick_sort_seq_inplace(sequence, right_part_l, r);
}

void quick_sort_seq_inplace(parlay::sequence<int> &sequence) {
    quick_sort_seq_inplace(sequence, 0, sequence.size());
}

