#include "doctest/doctest.h"
#include "doctest/doctest_utils.h"

#include "std_e/algorithm/permutation.h"
#include <vector>

TEST_CASE("partial_accumulate") {
  std::vector<int> perm = {4,3,0,1,2};

  std::vector<int> inv_perm = std_e::invert_permutation(perm);

  std::vector<int> expected_inv_perm = {2,3,4,1,0};
  CHECK( inv_perm == expected_inv_perm);

  CHECK( std_e::invert_permutation(std_e::invert_permutation(perm)) == perm );
}
