#include <cassert>
#include <chrono>
#include <forward_list>
#include <iostream>
#include <memory>

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
  std::cout << '\n';

  // Destroy and deallocate memory
  for (int i = 0; i < 5; ++i) {
    allocator.destroy(&p1[i]);
  }
  allocator.deallocate(p1, 5);
}

// Test function for LinkedList with StackAllocator
void test_linked_list() {
  LinkedList<int> list{1, 2, 3};

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

template <typename ListType1, typename ListType2>
bool compare_lists(const ListType1& list1, const ListType2& list2) {
  auto it1 = list1.begin();
  auto it2 = list2.begin();

  while (it1 != list1.end() && it2 != list2.end()) {
    if (*it1 != *it2) {
      return false;
    }
    ++it1;
    ++it2;
  }
  return (it1 == list1.end() && it2 == list2.end());
}

void test_copy_constructor_and_assignment_operator() {
  std::cout << "\nTesting copy constructor and assignment operator..." << '\n';

  // Default allocator
  LinkedList<int> list1{1, 2, 3};
  LinkedList<int> list2 = list1;  // Copy constructor

  std::cout << "list1 (default allocator): ";
  for (auto elem : list1) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  std::cout << "list2 (copied from list1, default allocator): ";
  for (auto elem : list2) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  LinkedList<int> list3;
  list3 = list1;  // Assignment operator

  std::cout << "list3 (assigned from list1, default allocator): ";
  for (auto elem : list3) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  // Custom allocator
  LinkedList<int, StackAllocator<int>> list4{4, 5, 6};
  LinkedList<int, StackAllocator<int>> list5 = list4;  // Copy constructor

  std::cout << "list4 (custom allocator): ";
  for (auto elem : list4) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  std::cout << "list5 (copied from list4, custom allocator): ";
  for (auto elem : list5) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  LinkedList<int, StackAllocator<int>> list6;
  list6 = list4;  // Assignment operator

  std::cout << "list6 (assigned from list4, custom allocator): ";
  for (auto elem : list6) {
    std::cout << elem << " ";
  }
  std::cout << '\n';
}

void test_compare_with_std_forward_list() {
  std::cout << "\nTesting LinkedList against std::forward_list..." << '\n';

  LinkedList<int> custom_list{1, 2, 3};
  std::forward_list<int> std_list{1, 2, 3};

  // Compare initial state
  if (!compare_lists(custom_list, std_list)) {
    std::cout << "Initial lists do not match!\n";
    std::cout << "Custom list: ";
    for (auto elem : custom_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';

    std::cout << "std::forward_list: ";
    for (auto elem : std_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';
  }
  assert(compare_lists(custom_list, std_list));
  std::cout << "Initial lists are equal.\n";

  // Push elements
  custom_list.push_front(4);
  std_list.push_front(4);
  if (!compare_lists(custom_list, std_list)) {
    std::cout << "After push_front(4), lists do not match!\n";
    std::cout << "Custom list: ";
    for (auto elem : custom_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';

    std::cout << "std::forward_list: ";
    for (auto elem : std_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';
  }
  assert(compare_lists(custom_list, std_list));
  std::cout << "After push_front(4), lists are equal.\n";

  custom_list.push_front(5);
  std_list.push_front(5);
  if (!compare_lists(custom_list, std_list)) {
    std::cout << "After push_front(5), lists do not match!\n";
    std::cout << "Custom list: ";
    for (auto elem : custom_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';

    std::cout << "std::forward_list: ";
    for (auto elem : std_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';
  }
  assert(compare_lists(custom_list, std_list));
  std::cout << "After push_front(5), lists are equal.\n";

  // Pop elements
  custom_list.pop_front();
  std_list.pop_front();
  if (!compare_lists(custom_list, std_list)) {
    std::cout << "After pop_front(), lists do not match!\n";
    std::cout << "Custom list: ";
    for (auto elem : custom_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';

    std::cout << "std::forward_list: ";
    for (auto elem : std_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';
  }
  assert(compare_lists(custom_list, std_list));
  std::cout << "After pop_front(), lists are equal.\n";

  custom_list.pop_front();
  std_list.pop_front();
  if (!compare_lists(custom_list, std_list)) {
    std::cout << "After another pop_front(), lists do not match!\n";
    std::cout << "Custom list: ";
    for (auto elem : custom_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';

    std::cout << "std::forward_list: ";
    for (auto elem : std_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';
  }
  assert(compare_lists(custom_list, std_list));
  std::cout << "After another pop_front(), lists are equal.\n";

  // Clear lists
  custom_list.clear();
  std_list.clear();
  if (!compare_lists(custom_list, std_list)) {
    std::cout << "After clear(), lists do not match!\n";
    std::cout << "Custom list: ";
    for (auto elem : custom_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';

    std::cout << "std::forward_list: ";
    for (auto elem : std_list) {
      std::cout << elem << " ";
    }
    std::cout << '\n';
  }
  assert(compare_lists(custom_list, std_list));
  std::cout << "After clear(), lists are equal.\n";
}

void test_copy_constructor_and_assignment() {
  // Test std::forward_list
  std::forward_list<int, std::allocator<int>> std_list1{1, 2, 3};
  std::forward_list<int, std::allocator<int>> std_list2(
      std_list1);  // Copy constructor
  std::forward_list<int, std::allocator<int>> std_list3;
  std_list3 = std_list1;  // Assignment operator

  // Test LinkedList with default allocator
  LinkedList<int> linked_list1{1, 2, 3};
  LinkedList<int> linked_list2(linked_list1);  // Copy constructor
  LinkedList<int> linked_list3;
  linked_list3 = linked_list1;  // Assignment operator

  // Print lists
  std::cout << "std::forward_list 1: ";
  for (auto elem : std_list1) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  std::cout << "std::forward_list 2 (copy): ";
  for (auto elem : std_list2) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  std::cout << "std::forward_list 3 (assignment): ";
  for (auto elem : std_list3) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  std::cout << "LinkedList 1: ";
  for (auto elem : linked_list1) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  std::cout << "LinkedList 2 (copy): ";
  for (auto elem : linked_list2) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  std::cout << "LinkedList 3 (assignment): ";
  for (auto elem : linked_list3) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  // Check that all lists are equal
  assert(compare_lists(std_list1, linked_list1));
  assert(compare_lists(std_list2, linked_list2));
  assert(compare_lists(std_list3, linked_list3));
}

// Test function for LinkedList with custom allocator
void test_linked_list_with_custom_allocator() {
  LinkedList<int, StackAllocator<int, 100>> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  list.push_back(4);

  std::cout << "Linked list with custom allocator: ";
  for (auto elem : list) {
    std::cout << elem << " ";
  }
  std::cout << '\n';

  list.pop_front();
  std::cout << "Linked list after popping: ";
  for (auto elem : list) {
    std::cout << elem << " ";
  }
  std::cout << '\n';
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

  test_copy_constructor_and_assignment_operator();
  test_compare_with_std_forward_list();

  std::cout << "Testing LinkedList with std::forward_list...\n";
  test_copy_constructor_and_assignment();

  std::cout << "Testing LinkedList with custom allocator...\n";
  test_linked_list_with_custom_allocator();

}
