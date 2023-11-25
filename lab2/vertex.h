#pragma once

#include <vector>

template <typename ID, typename SELF>
class Vertex {
public:

    virtual ID get_id() const = 0;

    virtual std::vector<SELF> get_neighbors() = 0;

    virtual SELF& safe_cast() = 0;

    virtual size_t calculate_graph_size() const = 0;

    virtual size_t calculate_index_in_graph() const = 0;
};

