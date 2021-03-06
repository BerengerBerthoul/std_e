#pragma once


#include "std_e/algorithm/permutation.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {


/**
*  class "rearranging_view"
*
*  Directly changing node positions when reordering a graph has to be done in two steps:
*    - change position of the node
*    - update its position for its inward and outward adjacencies
*  This is cumbersome. Plus, doing it for each reordering can be expansive,
*  since we also need to change all ins and outs connectivities
*
*  The idea here is to keep a permutation table and to always access adjacencies through an indirect indexing
*/
template<class Range, class I>
class rearranging_reference;

template<class Range, class I>
class rearranging_value {
  public:
  // type traits
    using difference_type = I;
    using reference = rearranging_reference<Range,I>;

  // ctors
    constexpr rearranging_value() = delete;
    constexpr rearranging_value(const rearranging_value&) = default;
    constexpr rearranging_value(rearranging_value&&) = default;
    constexpr rearranging_value& operator=(const rearranging_value& x);
    constexpr rearranging_value& operator=(rearranging_value&& x);

    constexpr
    rearranging_value(Range* r, I& i_ref)
      : r(r)
      , i_ref(i_ref)
    {}

    constexpr
    operator reference() const {
      throw;
    }

    auto
    underlying_ref() -> decltype(auto) {
      return (*r)[i_ref];
    }
    auto
    underlying_ref() const -> decltype(auto) {
      return (*r)[i_ref];
    }

    constexpr auto
    operator<=>(const rearranging_value& x) const = delete;
  private:
    Range* r;
    I& i_ref; // TODO ptr! (here: not regular semantics)
};

template<class Range, class I>
class rearranging_reference {
  public:
  // type traits
    using value_type = rearranging_value<Range,I>;
    using difference_type = I;

  // ctors
    constexpr rearranging_reference() = delete;
    constexpr rearranging_reference(const rearranging_reference&) = default;
    constexpr rearranging_reference(rearranging_reference&&) = default;
  // assignement for Reference
    constexpr
    rearranging_reference& operator=(const rearranging_reference& x) {
      STD_E_ASSERT(r==x.r);
      i_ref = x.i_ref;
      return *this;
    }
    constexpr
    rearranging_reference& operator=(rearranging_reference&& x) {
      STD_E_ASSERT(r==x.r);
      i_ref = x.i_ref;
      return *this;
    }
    constexpr
    rearranging_reference& operator=(const value_type& x) const {
      throw;
    }
    constexpr
    rearranging_reference& operator=(value_type& x) const {
      throw;
    }

    constexpr
    rearranging_reference(Range* r, I& i_ref)
      : r(r)
      , i_ref(i_ref)
    {}


    friend constexpr auto
    swap(rearranging_reference&& x, rearranging_reference&& y) -> void {
      std::swap(x.i_ref,y.i_ref);
    }
    friend constexpr auto
    swap(rearranging_reference& x, rearranging_reference& y) -> void {
      std::swap(x.i_ref,y.i_ref);
    }

  // get reference to underlying object
    auto
    underlying_ref() -> decltype(auto) {
      return (*r)[i_ref];
    }
    auto
    underlying_ref() const -> decltype(auto) {
      return (*r)[i_ref];
    }

    constexpr auto
    operator<=>(const rearranging_reference& x) const = delete;
  private:
    Range* r;
    I& i_ref;
};

template<class Range, class I>
rearranging_reference(Range* r, I& i) -> rearranging_reference<Range,I>;

template<class Range, class I> auto
to_string(const rearranging_reference<Range,I>& x) {
  return to_string(x.underlying_ref());
}


// TODO generate iterator from template
template<class Range, class I>
class rearranging_iterator {
  public:
  // type traits
    /// std::iterator type traits
    using difference_type = I;
    using value_type = rearranging_value<Range,difference_type>;
    using reference = rearranging_reference<Range,difference_type>;
    using iterator_category = std::random_access_iterator_tag;

  // ctors
    constexpr rearranging_iterator() = default;
    constexpr rearranging_iterator(const rearranging_iterator&) = default;
    constexpr rearranging_iterator& operator=(const rearranging_iterator&) = default;

    constexpr
    rearranging_iterator(Range* r, difference_type* perm_ptr)
      : r(r)
      , perm_ptr(perm_ptr)
    {}

  // iterator interface
    constexpr auto
    operator++() -> rearranging_iterator& {
      ++perm_ptr;
      return *this;
    }
    constexpr auto
    operator++(int) -> rearranging_iterator {
      rearranging_iterator it(*this);
      ++(*this);
      return it;
    }

    constexpr auto
    operator--() -> rearranging_iterator& {
      --perm_ptr;
      return *this;
    }
    constexpr auto
    operator--(int) -> rearranging_iterator {
      rearranging_iterator it(*this);
      --(*this);
      return it;
    }

    constexpr auto
    operator+=(difference_type i) -> rearranging_iterator& {
      perm_ptr += i;
      return *this;
    }
    constexpr auto
    operator-=(difference_type i) -> rearranging_iterator& {
      perm_ptr -= i;
      return *this;
    }
    constexpr auto
    operator+(difference_type i) const -> rearranging_iterator {
      return {r,perm_ptr+i};
    }
    friend constexpr auto
    operator+(difference_type i, const rearranging_iterator& x) -> rearranging_iterator {
      return {x.r,x.perm_ptr+i};
    }
    constexpr auto
    operator-(difference_type i) const -> rearranging_iterator {
      return {r,perm_ptr-i};
    }
    constexpr auto
    operator-(const rearranging_iterator& x) const -> difference_type {
      STD_E_ASSERT(r==x.r);
      return perm_ptr - x.perm_ptr;
    }

    constexpr auto
    operator*() const -> reference {
      return {r,*perm_ptr};
    }
    constexpr auto
    operator[](difference_type i) const -> reference {
      return {r,*(perm_ptr+i)};
    }
    auto
    operator->() const {
      return std_e::arrow_proxy<reference>{**this};
    }

    constexpr auto
    operator==(const rearranging_iterator& x) const {
      STD_E_ASSERT(r==x.r);
      return perm_ptr == x.perm_ptr;
    }
    constexpr auto
    operator<=>(const rearranging_iterator& x) const {
      STD_E_ASSERT(r==x.r);
      return perm_ptr <=> x.perm_ptr;
    }
  private:
    Range* r;
    difference_type* perm_ptr;
};

template<class Range> // TODO Range->range R
class rearranging_view {
  public:
    using difference_type = typename Range::difference_type;
    using reference = rearranging_reference<Range,difference_type>;
    using const_reference = rearranging_reference<Range,const difference_type>;
    using iterator_type = rearranging_iterator<Range,difference_type>;
    using const_iterator_type = rearranging_iterator<Range,const difference_type>;

    constexpr
    rearranging_view(Range& r)
      : r(&r)
      , perm(r.size())
    {
      std::iota(perm.begin(),perm.end(),0);
    }

    constexpr auto
    size() const -> difference_type {
      return perm.size();
    }
    constexpr auto
    resize(difference_type sz) -> void {
      STD_E_ASSERT(sz <= size());
      perm.resize(sz);
    }

    constexpr auto
    begin() -> iterator_type {
      return {r,perm.data()};
    }
    constexpr auto
    begin() const -> const_iterator_type {
      return {r,perm.data()};
    }
    constexpr auto
    end() -> iterator_type {
      return {r,perm.data()+perm.size()};
    }
    constexpr auto
    end() const -> const_iterator_type {
      return {r,perm.data()+perm.size()};
    }
    constexpr auto
    operator[](difference_type i) -> reference {
      return {r,perm[i]};
    }
    constexpr auto
    operator[](difference_type i) const -> const_reference {
      return {r,perm[i]};
    }

    auto
    old_to_new_indices() const {
      return inverse_partial_permutation(perm,initial_sz(),-1);
    }
  private:
  public: // TODO
    auto
    initial_sz() const -> difference_type {
      return r->size();
    }
    Range* r;
    std::vector<difference_type> perm;
};

template<class Range> constexpr auto
begin(rearranging_view<Range>& x) {
  return x.begin();
}
template<class Range> constexpr auto
begin(const rearranging_view<Range>& x) {
  return x.begin();
}
template<class Range> constexpr auto
end(rearranging_view<Range>& x) {
  return x.end();
}
template<class Range> constexpr auto
end(const rearranging_view<Range>& x) {
  return x.end();
}


} // std_e


template<class Range, class I>
struct std::iterator_traits<std_e::rearranging_iterator<Range,I>> {
  using type = std_e::rearranging_iterator<Range,I>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};

template<class Range, class I>
struct std::common_reference<std_e::rearranging_value<Range,I>&,std_e::rearranging_reference<Range,I>&&> {
  using type = std_e::rearranging_reference<Range,I>;
};
template<class Range, class I>
struct std::common_reference<std_e::rearranging_reference<Range,I>&&,std_e::rearranging_value<Range,I>&> {
  using type = std_e::rearranging_reference<Range,I>;
};
