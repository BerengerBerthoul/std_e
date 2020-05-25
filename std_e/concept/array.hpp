#pragma once


#include <array>
#include <vector>


namespace std_e {

/*
concept Array : Regular
  Regular value_type
  Pointer pointer_type
  Pointer const_pointer_type
  Reference reference_type
  Reference const_reference_type

  size() -> Integer

  data() -> pointer_type
  data() const -> const_pointer_type

  begin() -> pointer_type
  end() -> pointer_type
  [] -> reference_type
  [] const -> const_reference_type
*/
template<class T> struct enable_is_array : std::false_type {};
template<class T> constexpr bool is_array = enable_is_array<T>::value;


/*
concept Fixed_size_array : Array
  static constexpr size() // NOTE: unfortunately not the case with std::array (std:: bug, see https://stackoverflow.com/q/21936507/1583122)
*/
template<class T> struct enable_is_fixed_size_array : std::false_type {};
template<class T> inline constexpr bool is_fixed_size_array = enable_is_fixed_size_array<T>::value;


/*
concept Dyn_size_array : Array
  ctor(Integer size) // NOTE: a std::vector<T,A> that needs an allocator to be constructed is not a Dyn_size_array
  void push_back(value_type)
*/
template<class T> struct enable_is_dyn_size_array : std::false_type {};
template<class T> inline constexpr bool is_dyn_size_array = enable_is_dyn_size_array<T>::value;




// std::array activations
template<class T, int N>
struct enable_is_array<std::array<T,N>> : std::true_type {};
template<class T, int N>
struct enable_is_fixed_size_array<std::array<T,N>> : std::true_type {};

// std::vector activations
template<class T>
struct enable_is_array<std::vector<T>> : std::true_type {};
template<class T>
struct enable_is_dyn_size_array<std::vector<T>> : std::true_type {};


} // std_e
