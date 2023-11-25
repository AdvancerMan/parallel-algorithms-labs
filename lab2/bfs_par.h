#pragma once

#include <algorithm>
#include <atomic>

#include <parlay/sequence.h>

#include "vertex.h"

const int SEQ_SIZE = 500;

template <typename T, typename R>
parlay::sequence<R> map_par(parlay::sequence<T>& sequence, std::function<R(T&)> mapper) {
    parlay::sequence<R> result(sequence.size());
    parlay::blocked_for(0, sequence.size(), SEQ_SIZE, [&](size_t, size_t l, size_t r) {
        for (size_t j = l; j < r; j++) {
            result[j] = mapper(sequence[j]);
        }
    });
    return result;
}

parlay::sequence<size_t> scan_par(parlay::sequence<size_t> const& sequence, std::function<size_t(size_t, size_t)> operation) {
    if (sequence.size() == 0) {
        return sequence;
    } else if (sequence.size() <= SEQ_SIZE) {
        parlay::sequence<size_t> result(sequence.size());
        result[0] = sequence[0];
        for (size_t i = 1; i < sequence.size(); i++) {
            result[i] = operation(result[i - 1], sequence[i]);
        }
        return result;
    }
    
    parlay::sequence<size_t> block_result((sequence.size() + SEQ_SIZE - 1) / SEQ_SIZE);
    parlay::blocked_for(0, sequence.size(), SEQ_SIZE, [&](size_t i, size_t l, size_t r) {
        block_result[i] = 0;
        for (size_t j = l; j < r; j++) {
            block_result[i] = operation(block_result[i], sequence[j]);
        }
    });
    
    parlay::sequence<size_t> scanned_blocks = scan_par(block_result, operation);
    parlay::sequence<size_t> result(sequence.size());
    parlay::blocked_for(0, sequence.size(), SEQ_SIZE, [&](size_t i, size_t l, size_t r) {
        size_t current_result = i == 0 ? 0 : scanned_blocks[i - 1];
        for (size_t j = l; j < r; j++) {
            current_result = operation(current_result, sequence[j]);
            result[j] = current_result;
        }
    });
    
    return result;
}

template <typename ID, typename VERTEX>
int bfs_calculate_distance_par(Vertex<ID, VERTEX>& start, ID finish_id) {
    class BfsVertexDescriptor {
    public:

        BfsVertexDescriptor(VERTEX const& vertex, bool is_empty)
            : vertex(vertex)
            , is_empty_property(is_empty) {}
    
        BfsVertexDescriptor(BfsVertexDescriptor const& descriptor)
            : BfsVertexDescriptor(descriptor.vertex, descriptor.is_empty_property) {}

        VERTEX& get_vertex() {
            return vertex;
        }

        bool is_empty() {
            return is_empty_property;
        }

        void set_vertex(VERTEX const& vertex) {
            this->vertex = vertex;
            is_empty_property = false;
        }
        
    private:

        VERTEX vertex;
        bool is_empty_property;
    };

    parlay::sequence<std::atomic<bool>> visited_vertices(start.calculate_graph_size());
    parlay::sequence<BfsVertexDescriptor> frontiers(1, BfsVertexDescriptor(start.safe_cast(), false));

    int current_distance = 0;
    while (!frontiers.empty()) {
        parlay::sequence<size_t> degree = map_par<BfsVertexDescriptor, size_t>(frontiers, [](BfsVertexDescriptor& descriptor) {
            if (descriptor.is_empty()) {
                return static_cast<size_t>(0);
            }

            return descriptor.get_vertex().get_neighbors().size();
        });

        parlay::sequence<size_t> scanned_degree = scan_par(degree, std::plus<size_t>());
        parlay::sequence<BfsVertexDescriptor> next_frontiers(scanned_degree.back(), BfsVertexDescriptor(start.safe_cast(), true));
        
        bool finish_found = false;
        
        parlay::blocked_for(0, scanned_degree.back(), SEQ_SIZE, [&](size_t, size_t l, size_t r) {
            size_t frontier_l = std::upper_bound(scanned_degree.begin(), scanned_degree.end(), l) - scanned_degree.begin();
            for (size_t frontier_i = frontier_l; (frontier_i == 0 ? 0 : scanned_degree[frontier_i - 1]) < r; frontier_i++) {
                if (frontiers[frontier_i].is_empty()) {
                    continue;
                }

                size_t degree_sum_before = frontier_i == 0 ? 0 : scanned_degree[frontier_i - 1];
                size_t frontier_neighbor_l = l < degree_sum_before ? 0 : l - degree_sum_before;
                size_t frontier_neighbor_r = r < degree_sum_before ? 0 : std::min(degree[frontier_i], r - degree_sum_before);
                for (size_t frontier_neighbor_i = frontier_neighbor_l; frontier_neighbor_i < frontier_neighbor_r; frontier_neighbor_i++) {
                    VERTEX neighbor = frontiers[frontier_i].get_vertex().get_neighbors()[frontier_neighbor_i];
                    bool expected = false;
                    if (atomic_compare_exchange_strong(&visited_vertices[neighbor.calculate_index_in_graph()], &expected, true)) {
                        next_frontiers[frontier_neighbor_i + degree_sum_before].set_vertex(neighbor);

                        if (neighbor.get_id() == finish_id) {
                            finish_found = true;
                        }
                    }
                }
            }
        });

        frontiers.swap(next_frontiers);
        current_distance++;
        
        if (finish_found) {
            return current_distance;
        }
    }
    
    return -1;
}

