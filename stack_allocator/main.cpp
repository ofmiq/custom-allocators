#include <chrono>
#include <forward_list>
#include <iostream>

#include "LinkedList.h"
#include "StackAllocator.h"

// Test function for StackAllocator
void test_stack_allocator() {
  StackAllocator<int> allocator;

  // Allocate memory for 5 integers
  int* p1 = allocator.allocate(5);
  for (int i = 0; i < 5; ++i) {
    allocator.construct(&p1[i], i);
  }

  // Print the allocated and constructed integers
  std::cout << "Allocated integers: ";
  for (int i = 0; i < 5; ++i) {
    std::cout << p1[i] << " ";
  }
  std::cout << '\n';

  // Print the addresses of variables to make sure that is stack memory
  std::cout << "Addresses of variables: ";
  for (int i = 0; i < 5; ++i) {
    std::cout << &(p1[i]) << " ";
  }

  // Destroy and deallocate memory
  for (int i = 0; i < 5; ++i) {
    allocator.destroy(&p1[i]);
  }
  allocator.deallocate(p1, 5);
}

// Test function for LinkedList with StackAllocator
void test_linked_list() {
  LinkedList<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  // Print the linked list
  std::cout << "Linked list after pushing: ";
  for (auto elem : list) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  // Pop elements and print the list
  list.pop_front();
  list.pop_front();
  std::cout << "Linked list after popping: ";
  for (auto elem : list) {
    std::cout << elem << " ";
  }
  std::cout << '\n';
}

template <typename ListType>
void benchmark_list(ListType& list, int num_elements) {
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < num_elements; ++i) {
    list.push_front(i);
  }

  for (int i = 0; i < num_elements; ++i) {
    list.pop_front();
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;

  std::cout << "Time taken: " << duration.count() << " seconds" << '\n';
}

int main() {
  std::cout << "Testing StackAllocator..." << '\n';
  test_stack_allocator();

  std::cout << "\nTesting LinkedList with StackAllocator..." << '\n';
  test_linked_list();

  std::cout << "---------------\n";

  const int num_elements = 100'000;
  std::cout << "Benchmarking std::forward_list with std::allocator:" << '\n';
  std::forward_list<int, std::allocator<int>> forward_list_std_alloc;
  benchmark_list(forward_list_std_alloc, num_elements);

  std::cout << "Benchmarking LinkedList with std::allocator:" << '\n';
  LinkedList<int, std::allocator<int>> linked_list;
  benchmark_list(linked_list, num_elements);

  std::cout << "Benchmarking std::forward_list with StackAllocator:" << '\n';
  std::forward_list<int, StackAllocator<int, 100'010>> forward_list_cus_alloc;
  benchmark_list(forward_list_cus_alloc, num_elements);

  std::cout << "Benchmarking LinkedList with StackAllocator:" << '\n';
  LinkedList<int, StackAllocator<int, 100'010>> linked_list_cus_alloc;
  benchmark_list(linked_list_cus_alloc, num_elements);
}
