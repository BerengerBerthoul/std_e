#include "std_e/unit_test/doctest.hpp"
#include "std_e/parallel/serialize.hpp"
#include <iostream> // TODO


using namespace std_e;


struct S_serialize_test {
  int i;
  double d;
};


TEST_CASE("trivial serialize") {
  S_serialize_test s = {42,3.14};
  auto s_serial = serialize(s);

  // 0. check no copy, just a serialized view
  static_assert( std::is_same_v< decltype(serialize(s)) ,  span<const std::byte,sizeof(s)> > );

  // 1. check that the view points to the object's memory
  CHECK( (void*)s_serial.data() == (void*)&s );

  // 2 check size
  CHECK( s_serial.size() == sizeof(s) );

  // 3. check that data is at the right place
  const std::byte* s_serial_first_attibute = s_serial.data();
  auto s_serial_i = (const int*)s_serial_first_attibute;
  CHECK( s_serial_i == &s.i );
  CHECK( *s_serial_i == 42 );

  const std::byte* s_serial_second_attibute = s_serial.data() + alignof(s); // WARNING: alignof(s), NOT sizeof(int)
  auto s_serial_d = (const double*)s_serial_second_attibute;
  CHECK( s_serial_d == &s.d );
  CHECK( *s_serial_d == 3.14 );
}

TEST_CASE("trivial deserialize") {
  S_serialize_test s = {42,3.14};
  auto s_serial = serialize(s);

  S_serialize_test s_copy;
  deserialize(s_serial,s_copy);

  CHECK( s_copy.i == 42 );
  CHECK( s_copy.d == 3.14 );
}


TEST_CASE("serialize/deserialize array of trivial type") {
  std::vector<double> v = {3.14, 2.7, 1.618};
  auto v_serial = serialize(v);

  SUBCASE("serialize") {
    // 0. check no copy, just a serialized view
    static_assert( std::is_same_v< decltype(serialize(v)) ,  span<const std::byte> > );

    // 1. check that the view points to the object's memory
    CHECK( (void*)v_serial.data() == (void*)v.data() );

    // 2 check size
    CHECK( v_serial.size() == v.size()*sizeof(double) );

    // 3. check that data is at the right place
    const std::byte* s_serial_first = v_serial.data();
    auto v_serial_0 = (const double*)s_serial_first;
    CHECK( v_serial_0 == &v[0] );
    CHECK( *v_serial_0 == 3.14 );

    const std::byte* s_serial_second = v_serial.data() + sizeof(double);
    auto v_serial_1 = (const double*)s_serial_second;
    CHECK( v_serial_1 == &v[1] );
    CHECK( *v_serial_1 == 2.7 );

    const std::byte* s_serial_third = v_serial.data() + 2*sizeof(double);
    auto v_serial_2 = (const double*)s_serial_third;
    CHECK( v_serial_2 == &v[2] );
    CHECK( *v_serial_2 == 1.618 );
  }

  SUBCASE("deserialize") {
    std::vector<double> v_copy;
    deserialize(v_serial,v_copy);

    CHECK( v_copy == std::vector{3.14, 2.7, 1.618} );
  }
}


auto
fill_array_from_remote_call(std::byte* ptr, int size) -> void {
  auto i_ptr = (int*)ptr;
  int i_size = size/sizeof(int);
  for (int i=0; i<i_size; ++i) {
    *i_ptr++ = i*i;
  }
}

TEST_CASE("deserialize from function") {
  // The idea is that for a trivial type or an array of trivial type,
  // we don't need to 
  //   - allocate an array of std::byte,
  //   - call on it the function that gets us the serialized data
  //   - memcopy that data from the array into the final type
  // instead we can simply fill the final type, viewed as an array of std::byte
  //
  // A typical use would be during an MPI receive where we want
  // to put the data in the final type without the use of any intermediate buffer

  SUBCASE("array of trivial type") {
    std::vector<int> v;
    deserialize(fill_array_from_remote_call,sizeof(int)*10,v);

    CHECK( v.size() == 10 );
    CHECK( v == std::vector{0,1,4,9,16,25,36,49,64,81} );
  }
}


TEST_CASE("serialize/deserialize non-trivial type") {
  using data_t = std::vector<std::vector<int>>;
  const data_t x = {
    {1,2,3},
    {4,5,6,7},
    {8},
    {},
    {9,10}
  };
  auto x_serial = serialize(x);

  data_t y;
  deserialize(std_e::make_span(x_serial),y);

  CHECK( x == y );
}
