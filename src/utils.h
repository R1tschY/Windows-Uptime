#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <iterator>

namespace WinUptime {

template<typename T>
class range_iterator : public std::iterator<std::random_access_iterator_tag, T, T, const T*, T>
{
public:
  constexpr explicit range_iterator(T cursor)
    : iter_(cursor)
  { }

  constexpr range_iterator& operator++()
  {
    iter_ += T(1);
    return *this;
  }

  constexpr range_iterator& operator+=(std::size_t n)
  {
    iter_ += T(n);
    return *this;
  }

  constexpr T operator*() const { return iter_; }

  constexpr bool operator!=(const range_iterator& other)
  {
    return iter_ != other.iter_;
  }

  constexpr bool operator==(const range_iterator& other)
  {
    return iter_ == other.iter_;
  }

private:
  T iter_;
};

template<typename T>
class range
{
public:
  using iterator = range_iterator<T>;
  using const_iterator = range_iterator<T>;

  constexpr explicit range(T end)
    : start_(T(0)), end_(end)
  { }

  constexpr explicit range(T start, T end)
    : start_(start), end_(end)
  { }

  constexpr iterator begin() const { return range_iterator<T>(start_); }
  constexpr iterator end() const { return range_iterator<T>(end_); }

private:
  T start_;
  T end_;
};

using irange = range<int>;



} // namespace WinUptime

#endif // UTILS_H
