#pragma once


#include "std_e/multi_index/multi_index.hpp"


namespace std_e {


template<class Multi_index_0, class Multi_index_1> constexpr auto
increment_multi_index_fortran_order(const Multi_index_0& dims, Multi_index_1& indices) -> int {
  int rank = dims.size();
  if (rank==0) return 1;
  int i=0;
  while (++indices[i]==dims[i]) {
    indices[i]=0;
    ++i;
    if (i==rank) return rank;
  }
  return i;
}
template<class Multi_index_0, class Multi_index_1> constexpr auto
increment_multi_index_c_order(const Multi_index_0& dims, Multi_index_1& indices) -> int {
  int rank = dims.size();
  if (rank==0) return 1;
  int i=0;
  while (++indices[rank-1-i]==dims[rank-1-i]) {
    indices[rank-1-i]=0;
    ++i;
    if (i==rank) return rank;
  }
  return i;
}
template<class Multi_index_0, class Multi_index_1, class Multi_index_2> constexpr auto
increment_multi_index(const Multi_index_0& dims, Multi_index_1& indices, Multi_index_2& order) -> int {
  int rank = dims.size();
  if (rank==0) return 1;
  int i=0;
  while (++indices[order[i]]==dims[order[i]]) {
    indices[order[i]]=0;
    ++i;
    if (i==rank) return rank;
  }
  return i;
}


template<class Multi_index, class I> constexpr auto
multi_index_fortran_order(const Multi_index& dims, I idx_1d) -> Multi_index {
  Multi_index res = {};
  int rank = dims.size();
  for (int i=0; i<rank; ++i) {
    int div = dims[i];
    int rem = idx_1d % div;
    res[i] = rem;
    idx_1d /= div;
  }
  return res;
}
template<class Multi_index, class I> constexpr auto
multi_index_c_order(const Multi_index& dims, I idx_1d) -> Multi_index {
  Multi_index res = {};
  int rank = dims.size();
  for (int i=0; i<rank; ++i) {
    int div = dims[rank-1-i];
    int rem = idx_1d % div;
    res[rank-1-i] = rem;
    idx_1d /= div;
  }
  return res;
}


} // std_e
