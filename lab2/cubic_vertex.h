#pragma once

#include <tuple>

#include "vertex.h"

class CubicVertex : public Vertex<std::tuple<int, int, int>, CubicVertex> {
public:

    CubicVertex(CubicVertex const& other);

    CubicVertex(int cube_edge_size, std::tuple<int, int, int> id);

    CubicVertex& operator=(CubicVertex const& other);

    virtual std::tuple<int, int, int> get_id() const;

    virtual int get_cube_edge_size() const;

    virtual CubicVertex& safe_cast();

    virtual std::vector<CubicVertex> get_neighbors();

    virtual size_t calculate_graph_size() const;

    virtual size_t calculate_index_in_graph() const;

private:

    int cube_edge_size;
    std::tuple<int, int, int> id;
    std::vector<CubicVertex> neighbors_cache;
};

