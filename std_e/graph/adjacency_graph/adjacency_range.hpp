#pragma once


#include "std_e/graph/adjacency_graph/traits/traits.hpp"


namespace std_e {


template<class adjacency_graph_type, adj_orientation orientation>
class adjacency_range {
  public:
    using index_type = typename adjacency_graph_traits<adjacency_graph_type>::index_type;
    using adjacency_type = typename adjacency_graph_traits<adjacency_graph_type>::adjacency_type;
    using const_adjacency_type = typename adjacency_graph_traits<adjacency_graph_type>::const_adjacency_type;
    using adjacency_iterator_type = adjacency_connection_iterator<adjacency_graph_type,orientation>;
    using const_adjacency_iterator_type = adjacency_connection_iterator<const adjacency_graph_type,orientation>;

    constexpr
    adjacency_range() = default;

    constexpr
    adjacency_range(adjacency_graph_type* g, index_type node_idx)
      : g(g)
      , node_idx(node_idx)
    {}

    constexpr auto
    size() const -> index_type {
      if constexpr (orientation==adj_orientation::none) {
        return g->degree(node_idx);
      } else if constexpr (orientation==adj_orientation::in) {
        return g->in_degree(node_idx);
      } else {
        return g->out_degree(node_idx);
      }
    }

    constexpr auto
    begin() -> adjacency_iterator_type {
      return {g,node_idx,0};
    }
    constexpr auto
    begin() const -> const_adjacency_iterator_type {
      return {g,node_idx,0};
    }
    constexpr auto
    end() -> adjacency_iterator_type {
      return {g,node_idx,size()};
    }
    constexpr auto
    end() const -> const_adjacency_iterator_type {
      return {g,node_idx,size()};
    }

    constexpr auto
    operator[](index_type i) -> adjacency_type {
      return {g,index(i)};
    }
    constexpr auto
    operator[](index_type i) const -> const_adjacency_type {
      return {g,index(i)};
    }
  private:
  // functions
    constexpr auto
    index(index_type i) const -> index_type {
      if constexpr (orientation==adj_orientation::none) {
        return g->adjacency_list    ()[node_idx][i];
      } else if constexpr (orientation==adj_orientation::in) {
        return g->in_adjacency_list ()[node_idx][i];
      } else {
        return g->out_adjacency_list()[node_idx][i];
      }
    }
  // data
    adjacency_graph_type* g;
    index_type node_idx;
};


} // std_e
