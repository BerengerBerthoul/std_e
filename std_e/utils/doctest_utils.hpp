#pragma once


#include <vector>
#include "doctest/doctest.h"
#include "std_e/utils/to_string.hpp"


namespace doctest {


template<class T> struct StringMaker<std::vector<T>> {
  static String convert(const std::vector<T>& v) {
    std::string s = std_e::range_to_string(v);
    return s.c_str();
  }
};
template<class T, size_t N> struct StringMaker<std::array<T,N>> {
  static String convert(const std::array<T,N>& v) {
    std::string s = std_e::range_to_string(v);
    return s.c_str();
  }
};


} // doctest
