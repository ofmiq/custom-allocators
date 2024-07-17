#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include <cstddef>
#include <stdexcept>

template <typename T, size_t N = 1024>
class StackAllocator {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using void_pointer = void*;
  using const_void_pointer = const void*;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  template <typename U>
  struct rebind {
    using other = StackAllocator<U, N>;
  };

  StackAllocator() : offset_(0) {}

  pointer allocate(size_type n) {
    if (offset_ + n > N) {
      throw std::bad_alloc();
    }
    pointer p = reinterpret_cast<pointer>(&data_[offset_ * sizeof(value_type)]);
    offset_ += n;
    return p;
  }

  void deallocate(pointer, size_type) {
    // Simplified for demonstration purposes
  }

  template <typename U, typename... Args>
  void construct(U* p, Args&&... args) {
    new (p) U(std::forward<Args>(args)...);
  }

  template <typename U>
  void destroy(U* p) {
    p->~U();
  }

 private:
  alignas(alignof(value_type)) char data_[N * sizeof(value_type)];
  size_t offset_;
};

#endif  // STACK_ALLOCATOR_H
