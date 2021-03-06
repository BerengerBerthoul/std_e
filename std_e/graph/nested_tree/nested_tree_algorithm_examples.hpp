#pragma once


#include "std_e/graph/nested_tree/nested_tree.hpp"


namespace std_e {


template<class T, template<class> class M> auto
to_string__recursive_dfs_with_preordering(const nested_tree_base<T,M>& t, std::string indent = "") -> std::string {
  using std::to_string;
  std::string s = indent+to_string(root(t))+"\n";
  for (const auto& c : children(t)) {
    s += to_string__recursive_dfs_with_preordering(c,indent+unit_indent);
  }
  return s;
}
template<class T, template<class> class M> auto
to_string__recursive_dfs_with_postordering(const nested_tree_base<T,M>& t, std::string indent = "") -> std::string {
  using std::to_string;
  std::string s = "";
  for (const auto& c : children(t)) {
    s += to_string__recursive_dfs_with_postordering(c,indent+unit_indent);
  }
  return s+indent+to_string(root(t))+"\n";;
}


} // std_e
