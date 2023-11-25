#include <stddef.h>

#include "cubic_vertex.h"

CubicVertex::CubicVertex(int cube_edge_size, std::tuple<int, int, int> id)
    : cube_edge_size(cube_edge_size)
    , id(id)
    , neighbors_cache() {}

CubicVertex::CubicVertex(CubicVertex const& other)
    : CubicVertex(other.cube_edge_size, other.id) {}
    
CubicVertex& CubicVertex::operator=(CubicVertex const& other) {
    this->cube_edge_size = other.get_cube_edge_size();
    this->id = other.get_id();
    this->neighbors_cache.clear();
    return *this;
}

std::tuple<int, int, int> CubicVertex::get_id() const {
    return this->id;
}

int CubicVertex::get_cube_edge_size() const {
    return this->cube_edge_size;
}

std::vector<CubicVertex> CubicVertex::get_neighbors() {
    if (!neighbors_cache.empty()) {
        return this->neighbors_cache;
    }
    neighbors_cache.reserve(6);

    if (std::get<0>(this->id) - 1 >= 0) {
        neighbors_cache.push_back(CubicVertex(this->cube_edge_size, {std::get<0>(this->id) - 1, std::get<1>(this->id), std::get<2>(this->id)}));
    }
    if (std::get<0>(this->id) + 1 < this->cube_edge_size) {
        neighbors_cache.push_back(CubicVertex(this->cube_edge_size, {std::get<0>(this->id) + 1, std::get<1>(this->id), std::get<2>(this->id)}));
    }

    if (std::get<1>(this->id) - 1 >= 0) {
        neighbors_cache.push_back(CubicVertex(this->cube_edge_size, {std::get<0>(this->id), std::get<1>(this->id) - 1, std::get<2>(this->id)}));
    }
    if (std::get<1>(this->id) + 1 < this->cube_edge_size) {
        neighbors_cache.push_back(CubicVertex(this->cube_edge_size, {std::get<0>(this->id), std::get<1>(this->id) + 1, std::get<2>(this->id)}));
    }

    if (std::get<2>(this->id) - 1 >= 0) {
        neighbors_cache.push_back(CubicVertex(this->cube_edge_size, {std::get<0>(this->id), std::get<1>(this->id), std::get<2>(this->id) - 1}));
    }
    if (std::get<2>(this->id) + 1 < this->cube_edge_size) {
        neighbors_cache.push_back(CubicVertex(this->cube_edge_size, {std::get<0>(this->id), std::get<1>(this->id), std::get<2>(this->id) + 1}));
    }

    return this->neighbors_cache;
}

CubicVertex& CubicVertex::safe_cast() {
    return *this;
}

size_t CubicVertex::calculate_graph_size() const {
    return this->cube_edge_size * this->cube_edge_size * this->cube_edge_size;
}

size_t CubicVertex::calculate_index_in_graph() const {
    return std::get<0>(this->id) + std::get<1>(this->id) * this->cube_edge_size + std::get<2>(this->id) * this->cube_edge_size * this->cube_edge_size;
}

