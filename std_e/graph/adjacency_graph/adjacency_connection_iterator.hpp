#pragma once


#include "std_e/graph/adjacency_graph/traits/traits.hpp"


namespace std_e {


template<class adjacency_graph_type, adj_orientation orientation>
class adjacency_connection_iterator {
  public:
    using index_type = typename adjacency_graph_traits<adjacency_graph_type>::index_type;
    using adjacency_type = typename adjacency_graph_traits<adjacency_graph_type>::adjacency_type;

    constexpr
    adjacency_connection_iterator() = default;

    constexpr
    adjacency_connection_iterator(adjacency_graph_type* g, index_type node_idx, index_type connection_idx)
      : g(g)
      , node_idx(node_idx)
      , connection_idx(connection_idx)
    {}

    constexpr auto
    operator++() -> adjacency_connection_iterator& {
      ++connection_idx;
      return *this;
    }

    constexpr auto
    operator*() const -> adjacency_type {
      return {g,index()};
    }

    friend constexpr auto
    operator==(const adjacency_connection_iterator& x, const adjacency_connection_iterator& y) -> bool {
      STD_E_ASSERT(x.g==y.g);
      STD_E_ASSERT(x.node_idx==y.node_idx);
      return x.connection_idx== y.connection_idx;
    }
    friend constexpr auto
    operator!=(const adjacency_connection_iterator& x, const adjacency_connection_iterator& y) -> bool {
      return !(x==y);
    }
  private:
  // functions
    constexpr auto
    index() const -> index_type {
      if constexpr (orientation==adj_orientation::none) {
        return g->adjacency_list ()[node_idx][connection_idx];
      } else if constexpr (orientation==adj_orientation::none) {
        return g->in_adjacency_list()[node_idx][connection_idx];
      } else {
        return g->out_adjacency_list()[node_idx][connection_idx];
      }
    }
  // data
    adjacency_graph_type* g;
    index_type node_idx;
    index_type connection_idx;
};


} // std_e
