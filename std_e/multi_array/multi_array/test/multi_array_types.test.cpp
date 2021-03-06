#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_array/multi_array/multi_array_types.hpp"
#include "std_e/multi_index/multi_index_range.hpp"


using namespace std;
using namespace std_e;


// [Sphinx Doc] dyn_multi_array {
TEST_CASE("dyn_multi_array") {
  dyn_multi_array<int,2> ma = {
    {1,2,3},
    {4,5,6}
  };

  CHECK( ma.rank() == 2 );

  CHECK( ma.size() == 6 );
  CHECK( ma.extent(0) == 2 );
  CHECK( ma.extent(1) == 3 );

  CHECK( ma(0,0) == 1 ); CHECK( ma(0,1) == 2 ); CHECK( ma(0,2) == 3 );
  CHECK( ma(1,0) == 4 ); CHECK( ma(1,1) == 5 ); CHECK( ma(1,2) == 6 );
}
// [Sphinx Doc] dyn_multi_array }


// [Sphinx Doc] dyn_multi_array from dimensions {
TEST_CASE("dyn_multi_array from dimensions") {
  dyn_multi_array<int,2> ma(2,3);

  CHECK( ma.extent(0) == 2 );
  CHECK( ma.extent(1) == 3 );

  ma(0,0) = 1;    ma(0,1) = 2;    ma(0,2) = 3;
  ma(1,0) = 4;    ma(1,1) = 5;    ma(1,2) = 6;

  CHECK( ma(0,0) == 1 ); CHECK( ma(0,1) == 2 ); CHECK( ma(0,2) == 3 );
  CHECK( ma(1,0) == 4 ); CHECK( ma(1,1) == 5 ); CHECK( ma(1,2) == 6 );
}
// [Sphinx Doc] dyn_multi_array from dimensions }


TEST_CASE("dyn_multi_array_view") {
  // [Sphinx Doc] dyn_multi_array_view {
  vector<int> v = {1,2,3,4,5,6};
  dyn_multi_array_view<int,2> mav = {v.data(),{3,2}};
  // [Sphinx Doc] dyn_multi_array_view }

  SUBCASE("basic_tests") {

    CHECK( mav.size() == 6);
    CHECK( mav.extent(0) == 3);
    CHECK( mav.extent(1) == 2);

    CHECK( mav(0,0) == 1 );
    CHECK( mav(1,0) == 2 );
    CHECK( mav(2,0) == 3 );
    CHECK( mav(0,1) == 4 );
    CHECK( mav(1,1) == 5 );
    CHECK( mav(2,1) == 6 );
  }

  SUBCASE("using_external_memory") {
    mav(0,0) = 10;
    CHECK( v[0] == 10 );
    CHECK( v[1] ==  2 );
    CHECK( v[2] ==  3 );
    CHECK( v[3] ==  4 );
    CHECK( v[4] ==  5 );
    CHECK( v[5] ==  6 );
  }
}

// [Sphinx Doc] fixed_multi_array {
TEST_CASE("fixed_multi_array") {
  using ma_type = fixed_multi_array<int,2,3>;
  ma_type ma = {{1,2,3},{4,5,6}};

  CHECK( ma_type::rank() == 2 );
  CHECK( ma_type::size() == 6 );
  CHECK( ma_type::extent(0) == 2 );
  CHECK( ma_type::extent(1) == 3 );

  CHECK( ma.rank() == 2 );
  CHECK( ma.size() == 6 );
  CHECK( ma.extent(0) == 2 );
  CHECK( ma.extent(1) == 3 );

  CHECK( ma(0,0) == 1 ); CHECK( ma(0,1) == 2 ); CHECK( ma(0,2) == 3 );
  CHECK( ma(1,0) == 4 ); CHECK( ma(1,1) == 5 ); CHECK( ma(1,2) == 6 );
}
// [Sphinx Doc] fixed_multi_array }

// [Sphinx Doc] multi_array range interface {
TEST_CASE("multi_array range interface") {
  dyn_multi_array<double,2> ma = {
    {1.,2.,3.},
    {4.,5.,6.}
  };

  SUBCASE("iteration") {
    for (double& elt : ma) {
      elt += 10.;
    }

    CHECK( ma(0,0) == 11. ); CHECK( ma(0,1) == 12. ); CHECK( ma(0,2) == 13. );
    CHECK( ma(1,0) == 14. ); CHECK( ma(1,1) == 15. ); CHECK( ma(1,2) == 16. );
  }

  SUBCASE("random access") {
    CHECK( ma[3] == 5. );
  }

  SUBCASE("data()") {
    double* ptr = ma.data();
    CHECK( *(ptr+3) == 5. );
  }

  SUBCASE("underlying_range()") {
    std::vector<double>& v = ma.underlying_range();
    CHECK( v[3] == 5. );

    v[0] = 100.;
    CHECK( ma(0,0) == 100. );
  }
}
// [Sphinx Doc] multi_array range interface }

TEST_CASE("dyn_multi_array equality") {
  vector<int> v = {1,2,3,4,5,6};
  multi_index<int,2> dims = {3,2};

  dyn_multi_array_view<int,2> x = {v.data(),dims};

  dyn_multi_array<int,2> y({1,2,3,4,5,6},dims);
  dyn_multi_array<int,2> z({1,2,3,4,0,6},dims);

  CHECK(x==y);
  CHECK(x!=z);
}


TEST_CASE("multi_arrays of rank 0 are scalars") {
  SUBCASE("fixed array") {
    fixed_multi_array<int> f;
    CHECK( f.rank() == 0 );
    CHECK( f.size() == 1 );

    f() = 42;
    CHECK( f() == 42 );
  }
  SUBCASE("dyn array") {
    // The handling of dyn arrays of rank 0 is not well supported
    // because it would require special cases
    // whereas it seems not very useful

    dyn_multi_array<int,0> f_no_alloc;
    CHECK( f_no_alloc.rank() == 0 );
    CHECK( f_no_alloc.size() == 1 );

    // WARNING: nothing has been allocated!
    // The following would segfault
    // f_no_alloc() = 42; // BAD: SEGFAULT
  }
}

// [Sphinx Doc] make_view {
TEST_CASE("make_view") {
  dyn_multi_array<int,2> ma = {
    {1,2,3},
    {4,5,6}
  };

  auto view = make_view(ma);

  CHECK( ma.extent(0) == 2 );
  CHECK( ma.extent(1) == 3 );
  CHECK( view(0,0) == 1 ); CHECK( view(0,1) == 2 ); CHECK( view(0,2) == 3 );
  CHECK( view(1,0) == 4 ); CHECK( view(1,1) == 5 ); CHECK( view(1,2) == 6 );

  SUBCASE("changing the view changes the parent array") {
    view(0,1) = 42;
    CHECK( ma(0,1) == 42 );
  }

  SUBCASE("changing the parent array changes the view") {
    ma(1,2) = 1000;
    CHECK( view(1,2) == 1000 );
  }
}
// [Sphinx Doc] make_view }

// [Sphinx Doc] make_sub_array {
TEST_CASE("make_sub_array") {
  SUBCASE("one index") {
    dyn_multi_array<int,2> ma = {
      {1,2,3},
      {4,5,6}
    };

    auto sub_ma_0 = make_sub_array(ma,0);
    CHECK( sub_ma_0(0) == 1 );
    CHECK( sub_ma_0(1) == 4 );
    auto sub_ma_1 = make_sub_array(ma,1);
    CHECK( sub_ma_1(0) == 2 );
    CHECK( sub_ma_1(1) == 5 );
    auto sub_ma_2 = make_sub_array(ma,2);
    CHECK( sub_ma_2(0) == 3 );
    CHECK( sub_ma_2(1) == 6 );
  }
  SUBCASE("multi-index") {
    dyn_multi_array<double,3> ma(4,3,2);
    for (const auto& is : fortran_multi_index_range(ma.extent())) {
      ma(is) = 100*is[0] + 10*is[1] + is[2];
    }

    auto sub_ma_00 = make_sub_array(ma,{0,0});
    CHECK( sub_ma_00(0) ==   0. );
    CHECK( sub_ma_00(1) == 100. );
    CHECK( sub_ma_00(2) == 200. );
    CHECK( sub_ma_00(3) == 300. );
    auto sub_ma_11 = make_sub_array(ma,{1,1});
    CHECK( sub_ma_11(0) ==  11. );
    CHECK( sub_ma_11(1) == 111. );
    CHECK( sub_ma_11(2) == 211. );
    CHECK( sub_ma_11(3) == 311. );
  }
}
// [Sphinx Doc] make_sub_array }

// [Sphinx Doc] make_span {
TEST_CASE("make_span") {
  SUBCASE("one index") {
    dyn_multi_array<int,2> ma = {
      {1,2,3},
      {4,5,6}
    };

    auto span_0 = column(ma,0);
    CHECK( span_0[0] == 1 );
    CHECK( span_0[1] == 4 );
    auto span_1 = column(ma,1);
    CHECK( span_1[0] == 2 );
    CHECK( span_1[1] == 5 );
    auto span_2 = column(ma,2);
    CHECK( span_2[0] == 3 );
    CHECK( span_2[1] == 6 );
  }

  SUBCASE("multi-index") {
    dyn_multi_array<double,3> ma(4,3,2);
    for (const auto& is : fortran_multi_index_range(ma.extent())) {
      ma(is) = 100*is[0] + 10*is[1] + is[2];
    }
    auto sub_ma_00 = make_span(ma,{0,0});
    CHECK( sub_ma_00[0] ==   0. );
    CHECK( sub_ma_00[1] == 100. );
    CHECK( sub_ma_00[2] == 200. );
    CHECK( sub_ma_00[3] == 300. );
    auto sub_ma_11 = make_span(ma,{1,1});
    CHECK( sub_ma_11[0] ==  11. );
    CHECK( sub_ma_11[1] == 111. );
    CHECK( sub_ma_11[2] == 211. );
    CHECK( sub_ma_11[3] == 311. );
  }
}
// [Sphinx Doc] make_span }
