#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory>

#include "StackAllocator.h"

template <typename T, typename Alloc = StackAllocator<T>>
class LinkedList {
 public:
  struct Node {
    T data;
    Node* next;
  };

  using allocator_type =
      typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
  using value_type = typename std::allocator_traits<allocator_type>::value_type;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using size_type = typename std::allocator_traits<allocator_type>::size_type;

  LinkedList() : head_(nullptr), allocator_() {}

  LinkedList(const std::initializer_list<T>& list)
      : head_(nullptr), allocator_() {
    for (auto it = list.begin(); it != list.end(); ++it) {
      push_back(*it);
    }
  }

  LinkedList(const LinkedList& other)
      : head_(nullptr),
        allocator_(
            std::allocator_traits<allocator_type>::
                select_on_container_copy_construction(other.get_allocator())) {
    for (auto it = other.begin(); it != other.end(); ++it) {
      push_back(*it);
    }
  }
  ~LinkedList() { clear(); }

  LinkedList& operator=(const LinkedList& other) {
    if (this != &other) {
      clear();
      if (std::allocator_traits<
              allocator_type>::propagate_on_container_copy_assignment::value) {
        allocator_ = other.allocator_;
      }
      for (auto it = other.begin(); it != other.end(); ++it) {
        push_back(*it);
      }
    }
    return *this;
  }

  void push_front(const T& value) {
    pointer new_node = allocator_.allocate(1);
    std::allocator_traits<allocator_type>::construct(allocator_, new_node,
                                                     Node{value, head_});
    head_ = new_node;
  }

  void push_back(const T& value) {
    pointer new_node = allocator_.allocate(1);
    std::allocator_traits<allocator_type>::construct(allocator_, new_node,
                                                     Node{value, nullptr});
    if (head_ == nullptr) {
      head_ = new_node;
    } else {
      Node* temp = head_;
      while (temp->next) {
        temp = temp->next;
      }
      temp->next = new_node;
    }
  }

  void pop_front() {
    if (head_) {
      pointer temp = head_;
      head_ = head_->next;
      std::allocator_traits<allocator_type>::destroy(allocator_, temp);
      allocator_.deallocate(temp, 1);
    }
  }

  void clear() {
    while (head_) {
      pop_front();
    }
  }

  allocator_type get_allocator() const noexcept { return allocator_; }

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

    bool operator==(const iterator& other) const {
      return node_ == other.node_;
    }

   private:
    Node* node_;
  };

  iterator begin() const { return iterator(head_); }
  iterator end() const { return iterator(nullptr); }

 private:
  Node* head_;
  allocator_type allocator_;
};

#endif  // LINKED_LIST_H
