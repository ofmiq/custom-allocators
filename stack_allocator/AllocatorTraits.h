#ifndef ALLOCATOR_TRAITS_H
#define ALLOCATOR_TRAITS_H

template <typename Alloc>
struct AllocatorTraits {
  using allocator_type = Alloc;
  using value_type = typename std::allocator_traits<Alloc>::value_type;
  using pointer = typename std::allocator_traits<Alloc>::pointer;
  using size_type = typename std::allocator_traits<Alloc>::size_type;

  static pointer allocate(Alloc& alloc, size_type n) {
    return std::allocator_traits<Alloc>::allocate(alloc, n);
  }

  static void deallocate(Alloc& alloc, pointer p, size_type n) {
    std::allocator_traits<Alloc>::deallocate(alloc, p, n);
  }

  template <typename T, typename... Args>
  static void construct(Alloc& alloc, T* p, Args&&... args) {
    std::allocator_traits<Alloc>::construct(alloc, p,
                                            std::forward<Args>(args)...);
  }

  template <typename T>
  static void destroy(Alloc& alloc, T* p) {
    std::allocator_traits<Alloc>::destroy(alloc, p);
  }
};

#endif  // ALLOCATOR_TRAITS_H
