#ifndef UTILS_H
#define UTILS_H

#include <stdexcept>

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

class NullPointerException : public std::runtime_error {
public:
  NullPointerException() : std::runtime_error("used a null pointer") { }
};

template<typename T>
class CheckedPointer {
public:
  constexpr CheckedPointer() : ptr_(nullptr) { }
  constexpr CheckedPointer(T* ptr) : ptr_(ptr) { }

  T* get() {
    return *ptr_;
  }

  T& operator*() {
    if (ptr_)
      return *ptr_;
    else
      throw new NullPointerException();
  }

  T& operator->() {
    if (ptr_)
      return *ptr_;
    else
      throw new NullPointerException();
  }

  T& operator=(T* ptr) {
    ptr_ = ptr;
  }

private:
  T* ptr_;
};

#endif // UTILS_H

