#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "AllocatorTraits.h"
#include "StackAllocator.h"

template <typename T, typename Alloc = StackAllocator<T>>
class LinkedList {
 public:
  struct Node {
    T data;
    Node* next;
  };

  using allocator_type = typename AllocatorTraits<
      typename Alloc::template rebind<Node>::other>::allocator_type;
  using value_type = typename AllocatorTraits<
      typename Alloc::template rebind<Node>::other>::value_type;
  using pointer = typename AllocatorTraits<
      typename Alloc::template rebind<Node>::other>::pointer;
  using size_type = typename AllocatorTraits<
      typename Alloc::template rebind<Node>::other>::size_type;

  LinkedList() : head_(nullptr) {}

  ~LinkedList() { clear(); }

  void push_front(const T& value) {
    pointer new_node = allocator_.allocate(1);
    allocator_.construct(new_node, Node{value, head_});
    head_ = new_node;
  }

  void pop_front() {
    if (head_) {
      pointer temp = head_;
      head_ = head_->next;
      allocator_.destroy(temp);
      allocator_.deallocate(temp, 1);
    }
  }

  void clear() {
    while (head_) {
      pop_front();
    }
  }

  // Iterator for LinkedList
  class iterator {
   public:
    iterator(Node* node) : node_(node) {}

    T& operator*() { return node_->data; }

    iterator& operator++() {
      node_ = node_->next;
      return *this;
    }

    bool operator!=(const iterator& other) const {
      return node_ != other.node_;
    }

   private:
    Node* node_;
  };

  iterator begin() const { return iterator(head_); }

  iterator end() const { return iterator(nullptr); }

 private:
  Node* head_;
  typename Alloc::template rebind<Node>::other allocator_;
};

#endif  // LINKED_LIST_H
