#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/distribution.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("uniform_distribution") {
  SUBCASE("empty remainder") {
    int nb_slots = 3;
    std::vector<int> v(1+nb_slots);

    uniform_distribution(begin(v),end(v),100,15);
  // distributing 15 over 3 slots gives 5 + 5 + 5
  //                                    |   |   |
  //                                    v   v   v
    std::vector<int> v_expected = {100,105,110,115};
    CHECK( v == v_expected );
  }

  SUBCASE("non-empty remainder") {
    int nb_slots = 3;
    std::vector<int> v(1+nb_slots);

    uniform_distribution(begin(v),end(v),100,17);

  // distributing 17 over 3 slots gives 6 + 6 + 5
  //                                    |   |   |
  //                                    v   v   v
    std::vector<int> v_expected = {100,106,112,117};
    CHECK( v == v_expected );
  }

  SUBCASE("no slots") {
    int nb_slots = 0;
    std::vector<int> v(1+nb_slots);

    uniform_distribution(begin(v),end(v),100,0);

  // distributing  0 over 0 slots gives nothing
  //                                    |
  //                                    v
    std::vector<int> v_expected = {100};
    CHECK( v == v_expected );
  }

  SUBCASE("double specialisation") {
    int nb_slots = 4;
    std::vector<double> v(1+nb_slots);
    double ming = 0. ;
    double maxg = 10.;
    auto interval_size = maxg-ming;

    uniform_distribution(begin(v),end(v),ming,interval_size);
    std::vector<double> v_expected = {0.,2.5,5.,7.5,10.};
    CHECK( v == v_expected );
  }

  SUBCASE("double specialisation example 2 ") {
    int nb_slots = 1;
    std::vector<double> v(1+nb_slots);
    double ming = -14. ;
    double maxg =  18.;

    auto interval_size = maxg-ming;

    uniform_distribution(begin(v),end(v),ming,interval_size);
    std::vector<double> v_expected = {-14., 18.};
    CHECK( v == v_expected );
  }

}
