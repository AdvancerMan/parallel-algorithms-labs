#include <parlay/sequence.h>

#include "quick_sort_par.h"
#include "quick_sort_seq.h"

const int SEQ_SIZE = 1000;

parlay::sequence<int> map_par(parlay::sequence<int> const& sequence, std::function<int(int)> mapper) {
    parlay::sequence<int> result(sequence.size());
    parlay::blocked_for(0, sequence.size(), SEQ_SIZE, [&](size_t, size_t l, size_t r) {
        for (size_t j = l; j < r; j++) {
            result[j] = mapper(sequence[j]);
        }
    });
    return result;
}

parlay::sequence<int> scan_par(parlay::sequence<int> const& sequence, std::function<int(int, int)> operation) {
    if (sequence.size() == 0) {
        return sequence;
    } else if (sequence.size() <= SEQ_SIZE) {
        parlay::sequence<int> result(sequence.size());
        result[0] = sequence[0];
        for (size_t i = 1; i < sequence.size(); i++) {
            result[i] = operation(result[i - 1], sequence[i]);
        }
        return result;
    }
    
    parlay::sequence<int> block_result((sequence.size() + SEQ_SIZE - 1) / SEQ_SIZE);
    parlay::blocked_for(0, sequence.size(), SEQ_SIZE, [&](size_t i, size_t l, size_t r) {
        block_result[i] = 0;
        for (size_t j = l; j < r; j++) {
            block_result[i] = operation(block_result[i], sequence[j]);
        }
    });
    
    parlay::sequence<int> scanned_blocks = scan_par(block_result, operation);
    parlay::sequence<int> result(sequence.size());
    parlay::blocked_for(0, sequence.size(), SEQ_SIZE, [&](size_t i, size_t l, size_t r) {
        int current_result = i == 0 ? 0 : scanned_blocks[i - 1];
        for (size_t j = l; j < r; j++) {
            current_result = operation(current_result, sequence[j]);
            result[j] = current_result;
        }
    });
    
    return result;
}

parlay::sequence<int> pack_par(parlay::sequence<int> const& sequence,
                               parlay::sequence<int> const& flags_by_predicate) {
    if (sequence.size() == 0) {
        return sequence;
    } else if (sequence.size() <= SEQ_SIZE) {
        int flags_sum = 0;
        for (size_t i = 0; i < flags_by_predicate.size(); i++) {
            flags_sum += flags_by_predicate[i];
        }
        
        parlay::sequence<int> result(flags_sum);
        size_t result_i = 0;
        for (size_t i = 0; i < flags_by_predicate.size(); i++) {
            if (flags_by_predicate[i] == 0) {
                continue;
            }
            result[result_i++] = sequence[i];
        }
        return result;
    }

    parlay::sequence<int> block_sum((flags_by_predicate.size() + SEQ_SIZE - 1) / SEQ_SIZE);
    parlay::blocked_for(0, flags_by_predicate.size(), SEQ_SIZE, [&](size_t i, size_t l, size_t r) {
        block_sum[i] = 0;
        for (size_t j = l; j < r; j++) {
            block_sum[i] += flags_by_predicate[j];
        }
    });
    
    parlay::sequence<int> scanned_blocks = scan_par(block_sum, std::plus<int>());
    parlay::sequence<int> result(scanned_blocks[scanned_blocks.size() - 1]);
    parlay::blocked_for(0, flags_by_predicate.size(), SEQ_SIZE, [&](size_t i, size_t l, size_t r) {
        size_t result_i = i == 0 ? 0 : scanned_blocks[i - 1];
        for (size_t j = l; j < r; j++) {
            if (flags_by_predicate[j] == 0) {
                continue;
            }
            result[result_i++] = sequence[j];
        }
    });
    
    return result;
}

parlay::sequence<int> filter_par(parlay::sequence<int> const& sequence, std::function<bool(int)> predicate) {
    if (sequence.size() == 0) {
        return sequence;
    }

    parlay::sequence<int> flags_by_predicate = map_par(sequence, [&](int x) {
        return predicate(x);
    });

    return pack_par(sequence, flags_by_predicate);
}

parlay::sequence<int> quick_sort_par_theory_optimal(parlay::sequence<int> const& sequence) {
    if (sequence.size() <= SEQ_SIZE) {
        parlay::sequence<int> result(sequence);
        
        quick_sort_seq_inplace(result);
        return result;
    }

    size_t partition_index = sequence.size() / 2;
    int partition_value = sequence[partition_index];
    parlay::sequence<int> sorted_less_sequence;
    parlay::sequence<int> equal_sequence;
    parlay::sequence<int> sorted_greater_sequence;
    
    parlay::par_do([&] {
        sorted_less_sequence = quick_sort_par_theory_optimal(filter_par(sequence, [&](int x) { return x < partition_value; }));
    }, [&] {
        parlay::par_do([&] {
            sorted_greater_sequence = quick_sort_par_theory_optimal(filter_par(sequence, [&](int x) { return x > partition_value; }));
        }, [&] {
            equal_sequence = filter_par(sequence, [&](int x) { return x == partition_value; });
        });
    });

    parlay::sequence<int> result(sequence.size());
    parlay::blocked_for(0, sequence.size(), SEQ_SIZE, [&](size_t, size_t l, size_t r) {
        for (size_t j = l; j < r; j++) {
            if (j < sorted_less_sequence.size()) {
                result[j] = sorted_less_sequence[j];
            } else if (j - sorted_less_sequence.size() < equal_sequence.size()) {
                result[j] = equal_sequence[j - sorted_less_sequence.size()];
            } else {
                result[j] = sorted_greater_sequence[j - sorted_less_sequence.size() - equal_sequence.size()];
            }
        }
    });
    
    return result;
}

