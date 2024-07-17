#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STACK_SIZE 32
#define HEAP_SIZE (STACK_SIZE * 4)
#define HEADER 4

static uint16_t IN_USE = 0;

// Define a structure for managing virtual memory
typedef struct {
  uint8_t stack[STACK_SIZE];  // Stack memory area
  char** unmapped;            // Placeholder for unmapped memory
  uint8_t heap[HEAP_SIZE];    // Heap memory area

  struct {
    char** data;  // Placeholder for data section
    char** bss;   // Placeholder for bss section
    char** text;  // Placeholder for text section
  } data_t;
} VirtualMemory;

// Define a structure for memory entities
typedef struct {
  uint8_t* ptr;   // Pointer to the allocated memory block
  uint16_t size;  // Size of the allocated memory block
} Entity;

Entity LIST[40];  // List of memory entities (free-list)

// Function to log the current state of memory entities
void LOG() {
  printf("LIST:\n");
  for (int i = 0; i < IN_USE; ++i) {
    printf("Data + HEADER.[%p]. Memory of our heap free:[%u]\n",
           (void*)LIST[i].ptr, LIST[i].size);
  }
  printf("Entities in use:[%d]\n", IN_USE);
}

// Function to find or create a new memory entity for allocation
Entity* new_entity(size_t size) {
  // If the first entity is unused, initialize it with heap memory
  if (LIST[0].ptr == NULL && LIST[0].size == 0) {
    static VirtualMemory vm;
    LIST[0].ptr = vm.heap;
    LIST[0].size = HEAP_SIZE;
    ++IN_USE;
    LOG();
  }

  // Find the best fit memory entity that can accommodate 'size'
  Entity* best = NULL;
  uint16_t best_size = UINT16_MAX;

  for (int i = 0; i < IN_USE; ++i) {
    if (LIST[i].size >= size && LIST[i].size < best_size) {
      best = &LIST[i];
      best_size = LIST[i].size;
    }
  }

  // If no suitable entity is found, return NULL
  if (best == NULL) {
    return NULL;
  }

  return best;
}

// Function to allocate memory of a given size
void* c_malloc(size_t size) {
  // Check if the requested size exceeds the maximum heap size
  if (size > HEAP_SIZE - HEADER) {
    return NULL;
  }

  size += HEADER;  // Include header size for metadata

  // Try to find or create a suitable memory entity for allocation
  Entity* e = new_entity(size);

  if (e == NULL) {
    return NULL;  // Allocation failed
  }

  uint8_t* start = e->ptr;             // Start of allocated memory
  uint8_t* user_ptr = start + HEADER;  // User-accessible pointer

  *((uint16_t*)start) = size;  // Store the size at the beginning

  e->ptr += size;   // Move the entity pointer forward
  e->size -= size;  // Reduce the entity size

  // If the entity is fully used up, remove it from the list
  if (e->size == 0) {
    for (int i = 0; i < IN_USE; ++i) {
      if (&LIST[i] == e) {
        LIST[i] = LIST[IN_USE - 1];
        break;
      }
    }
    --IN_USE;
  }

  LOG();  // Log the current state of memory

  return user_ptr;  // Return the user-accessible pointer
}

// Function to free previously allocated memory
void c_free(void* ptr) {
  // Check if the pointer is real
  if (ptr == NULL) {
    return;
  }

  uint8_t* start = (uint8_t*)ptr - HEADER;  // Start of memory block
  uint16_t size = *((uint16_t*)start);      // Size of memory block

  assert(size > HEADER);  // Ensure size is greater than HEADER

  int insert_index = IN_USE;  // Index to insert freed block

  // Check if we can merge with the block before
  for (int i = 0; i < IN_USE; ++i) {
    if (LIST[i].ptr + LIST[i].size == start) {
      LIST[i].size += size;
      start = LIST[i].ptr;
      size = LIST[i].size;
      insert_index = i;
      break;
    }
  }

  // Check if we can merge with the block after
  if (insert_index == IN_USE) {
    for (int i = 0; i < IN_USE; ++i) {
      if (start + size == LIST[i].ptr) {
        start = LIST[i].ptr;
        size += LIST[i].size;
        insert_index = i;
        break;
      }
    }
  }

  // If no merge happened, insert the block as a new entry
  if (insert_index == IN_USE) {
    LIST[IN_USE].ptr = start;
    LIST[IN_USE].size = size;
    ++IN_USE;
  } else {
    // Adjust the merged block size in the list
    LIST[insert_index].ptr = start;
    LIST[insert_index].size = size;
  }

  // Compact the list by removing empty entries
  for (int i = 0; i < IN_USE - 1; ++i) {
    if (LIST[i].ptr + LIST[i].size == LIST[i + 1].ptr) {
      LIST[i].size += LIST[i + 1].size;
      for (int j = i + 1; j < IN_USE - 1; ++j) {
        LIST[j] = LIST[j + 1];
      }
      --IN_USE;
    }
  }

  LOG();  // Log the current state of memory
}

// Test function demonstrating memory allocation and deallocation
void test() {
  // Define a structure with integer and double members
  typedef struct {
    int a;
    double b;
  } Foo;

  // Allocate memory for Foo structure and initialize its members
  Foo* foo = (Foo*)c_malloc(sizeof(Foo));
  char* bar = (char*)c_malloc(5 * sizeof(char));
  int* bazz = (int*)c_malloc(sizeof(int));

  foo->a = 5;
  foo->b = 3.14;

  strcpy(bar, "bar");
  memcpy(bazz, &foo->a, sizeof(int));

  // Print allocated data
  printf("Address: [%p], data: [%d] [%e]\n", (void*)foo, foo->a, foo->b);
  printf("Address: [%p], data: [%s]\n", (void*)bar, bar);
  printf("Address: [%p], data: [%d]\n", (void*)bazz, *bazz);

  // Free allocated memory blocks
  c_free(foo);
  c_free(bar);
  c_free(bazz);

  // Allocate memory for a double and copy data from Foo structure
  double* fizz = (double*)c_malloc(sizeof(double));
  memcpy(fizz, &foo->b, sizeof(double));

  // Print allocated double data
  printf("Address: [%p], data: [%e]\n", (void*)fizz, *fizz);

  // Free allocated double memory
  c_free(fizz);
}

int main(int argc, char** argv) {
  test();  // Run the memory management test
  return 0;
}
