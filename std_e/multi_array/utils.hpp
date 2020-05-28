#pragma once


#include "std_e/base/not_implemented_exception.hpp"
#include "std_e/multi_array/shape/dyn_shape.hpp"
#include "std_e/future/span.hpp"
#include "std_e/multi_array/multi_array/multi_array.hpp"


namespace std_e {


template<class Integer, int rank> inline auto
reshape(dyn_shape<Integer,rank>& x, const multi_index<Integer,rank>& dims) {
  using multi_index_type = typename dyn_shape<Integer,rank>::multi_index_type;
  STD_E_ASSERT(x.offset()==make_zero_multi_index<multi_index_type>(x.offset().size()));
  x.extent() = dims;
  x.offset() = make_zero_multi_index<multi_index_type>(dims.size());
}

template<class Dynamic_contaier> auto
resize_memory(Dynamic_contaier& x, size_t n) -> void {
  x.resize(n);
}
template<class T, ptrdiff_t N> auto
resize_memory(span<T,N>& x, size_t n) -> void {
  // ASSERT same as or less than old size
}

template<class R, class Shape> auto
reshape(multi_array<R,Shape>& x, const typename Shape::multi_index_type& dims) {
  reshape(x.shape(),dims);
  size_t total_size = std_e::cartesian_product_size(dims);
  resize_memory(x.underlying_range(),total_size);
}


template<class R, class Shape> auto
is_empty(const multi_array<R,Shape>& x) -> bool {
  return x.size()==0;
}


template<class R, class Shape> auto
to_string(const multi_array<R,Shape>& x) -> std::string {
  using std::to_string;
  if constexpr (Shape::ct_rank==0 || Shape::ct_rank==dynamic_size) {
    if (x.rank()==0) {
      return "["+to_string(x())+"]";
    }
  }
  if constexpr (Shape::ct_rank==1 || Shape::ct_rank==dynamic_size) {
    if (x.rank()==1) {
      std::string s = "["+to_string(x(0));
      int n = x.extent(0);
      for (int i=1; i<n; ++i) {
        s += "," + std::to_string(x(i));
      }
      return s + "]";
    }
  }
  if constexpr (Shape::ct_rank==2 || Shape::ct_rank==dynamic_size) {
    if (x.rank()==2) {
      std::string s = "["+to_string(x(0,0));
      int n_i = x.extent(0);
      int n_j = x.extent(1);
      for (int j=1; j<n_j; ++j) {
        s += "," + std::to_string(x(0,j));
      }
      for (int i=1; i<n_i; ++i) {
        s += ";" + std::to_string(x(i,0));
        for (int j=1; j<n_j; ++j) {
          s += "," + std::to_string(x(i,j));
        }
      }
      return s + "]";
    }
  }
  throw not_implemented_exception("to_string(multi_array) implemented only for rank 1 and 2, not for rank "+std::to_string(x.rank()));
}


} // std_e
