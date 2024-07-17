#ifndef ALLOCATOR_TRAITS_H
#define ALLOCATOR_TRAITS_H

template <typename Alloc>
struct AllocatorTraits {
  using allocator_type = Alloc;
  using value_type = typename Alloc::value_type;
  using pointer = typename Alloc::pointer;
  using size_type = typename Alloc::size_type;

  static pointer allocate(Alloc& alloc, size_type n) {
    return alloc.allocate(n);
  }

  static void deallocate(Alloc& alloc, pointer p, size_type n) {
    alloc.deallocate(p, n);
  }

  template <typename T, typename... Args>
  static void construct(Alloc& alloc, T* p, Args&&... args) {
    alloc.construct(p, std::forward<Args>(args)...);
  }

  template <typename T>
  static void destroy(Alloc& alloc, T* p) {
    alloc.destroy(p);
  }
};

#endif  // ALLOCATOR_TRAITS_H
