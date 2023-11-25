#include <parlay/sequence.h>

std::pair<size_t, size_t> partition_seq_inplace(parlay::sequence<int> &sequence, int partition_value, size_t l, size_t r);

std::pair<size_t, size_t> partition_seq_inplace(parlay::sequence<int> &sequence, int partition_value);

void quick_sort_seq_inplace(parlay::sequence<int> &sequence, size_t l, size_t r);

void quick_sort_seq_inplace(parlay::sequence<int> &sequence);

