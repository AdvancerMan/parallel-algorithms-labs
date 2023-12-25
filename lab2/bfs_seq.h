#pragma once

#include <set>
#include <queue>

#include "vertex.h"

template <typename ID, typename VERTEX>
int* bfs_calculate_distances_seq(Vertex<ID, VERTEX>& start) {
    size_t graph_size = start.calculate_graph_size();
    int* distances = new int[graph_size];
    std::fill(distances, distances + graph_size, -1);
    
    std::queue<VERTEX> bfs_queue;
    bfs_queue.push(start.safe_cast());
    distances[start.calculate_index_in_graph()] = 0;
    
    while (!bfs_queue.empty()) {
        VERTEX current_vertex = bfs_queue.front();
        bfs_queue.pop();
        
        size_t current_index_in_graph = current_vertex.calculate_index_in_graph();
        int next_distance = distances[current_index_in_graph] + 1;
        
        for (VERTEX const& neighbor : current_vertex.get_neighbors()) {
            size_t neighbor_index_in_graph = neighbor.calculate_index_in_graph();
            if (distances[neighbor_index_in_graph] != -1) {
                continue;
            }

            distances[neighbor_index_in_graph] = next_distance;
            bfs_queue.push(neighbor);
        }
    }

    return distances;
}

