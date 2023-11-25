#pragma once

#include <set>
#include <queue>

#include "vertex.h"

template <typename ID, typename VERTEX>
int bfs_calculate_distance_seq(Vertex<ID, VERTEX>& start, ID finish_id) {
    class BfsVertexDescriptor {
    public:
    
        BfsVertexDescriptor(VERTEX const& vertex, int distance_from_start)
            : vertex(vertex)
            , distance_from_start(distance_from_start) {}
    
        BfsVertexDescriptor(BfsVertexDescriptor const& descriptor)
            : BfsVertexDescriptor(descriptor.vertex, descriptor.distance_from_start) {}

        VERTEX& get_vertex() {
            return vertex;
        }
        
        int get_distance_from_start() {
            return distance_from_start;
        }
        
    private:

        VERTEX vertex;
        int distance_from_start;
    };

    std::queue<BfsVertexDescriptor> next_descriptor_queue;
    std::set<ID> visited_ids;

    std::queue<BfsVertexDescriptor> descriptor_queue;
    std::set<ID> previous_visited_ids;

    next_descriptor_queue.push(BfsVertexDescriptor(start.safe_cast(), 0));
    visited_ids.insert(start.get_id());
    previous_visited_ids.insert(start.get_id());
    
    while (!descriptor_queue.empty() || !next_descriptor_queue.empty()) {
        if (descriptor_queue.empty()) {
            descriptor_queue.swap(next_descriptor_queue);
            previous_visited_ids.clear();
            previous_visited_ids.swap(visited_ids);
        }
    
        BfsVertexDescriptor current_descriptor = descriptor_queue.front();
        descriptor_queue.pop();
        visited_ids.insert(current_descriptor.get_vertex().get_id());
        
        int next_distance = current_descriptor.get_distance_from_start() + 1;
        
        for (VERTEX const& neighbor : current_descriptor.get_vertex().get_neighbors()) {
            if (visited_ids.count(neighbor.get_id()) + previous_visited_ids.count(neighbor.get_id()) > 0) {
                continue;
            }
            visited_ids.insert(neighbor.get_id());
            
            if (neighbor.get_id() == finish_id) {
                return next_distance;
            }
            
            next_descriptor_queue.push(BfsVertexDescriptor(neighbor, next_distance));
        }
    }
    
    return -1;
}

