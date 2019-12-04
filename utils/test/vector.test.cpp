#include "doctest/doctest.h"
#include "std_e/utils/vector.hpp"


TEST_CASE("sort_unique(vector)") {
  std::vector<int> v = {9,1,1,4,3,1,9,8,4,7};

  std_e::sort_unique(v);

  std::vector<int> expected_v = {1,3,4,7,8,9};
  CHECK( v == expected_v );
}
