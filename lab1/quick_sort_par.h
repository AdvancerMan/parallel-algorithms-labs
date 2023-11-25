#include <parlay/sequence.h>

void quick_sort_par_inplace(parlay::sequence<int> &sequence);

parlay::sequence<int> quick_sort_par_theory_optimal(parlay::sequence<int> const& sequence);

