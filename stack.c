/**
 * @file stack.c
 *
 * @brief Implementation of a generic stack.
 *
 * This file contains the internal implementation of the Stack defined in
 * stack.h. It uses a dynamically resized array and user-supplied functions for
 * element management.
 */

#include "stack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * Initial capacity for the stack.
 */
#define STACK_INITIAL_CAPACITY 8

/**
 * @struct Stack
 *
 * @brief Internal representation of the stack.
 *
 * Contains a dynamically allocated array of void pointers, current size, and
 * capacity. Element management is delegated to user-provided functions.
 */
struct Stack {
  void **data;             // Array of element pointers.
  size_t size;             // Current number of elements.
  size_t capacity;         // Allocated capacity.
  StackCopyFunc copy;      // Function to copy elements.
  StackFreeFunc free_func; // Function to free elements.
  StackCompareFunc cmp;    // Function to compare elements (optional).
};

/**
 * @brief Doubles the capacity of the stack.
 *
 * Called automatically when the stack runs out of space.
 *
 * Exits the program on allocation failure.
 *
 * @param stack Pointer to the Stack.
 */
static void grow(Stack *stack) {
  size_t new_capacity = stack->capacity ? stack->capacity * 2 : STACK_INITIAL_CAPACITY;
  void **new_data = realloc(stack->data, new_capacity * sizeof(void *));
  if (!new_data) {
    exit(EXIT_FAILURE);
  }
  stack->data = new_data;
  stack->capacity = new_capacity;
}

/**
 * @brief Initializes a new stack structure.
 * 
 * Allocates memory and sets initial values for fields.
 * 
 * No elements are present upon creation.
 * 
 * Exits the program on allocation failure.
 * 
 * @param copy_func Function to copy elements (must not be NULL).
 * @param free_func Function to free elements (must not be NULL).
 * @param cmp_func Function to compare elements (optional, may be NULL).
 *
 * @return Pointer to the new Stack.
 */
Stack *new_stack(StackCopyFunc copy_func, StackFreeFunc free_func, StackCompareFunc cmp_func) {
  Stack *stack = malloc(sizeof(Stack));
  if (!stack) {
    exit(EXIT_FAILURE);
  }
  stack->capacity = STACK_INITIAL_CAPACITY;
  stack->data = malloc(stack->capacity * sizeof(void *));
  if (!stack->data) {
    free(stack);
    exit(EXIT_FAILURE);
  }
  stack->size = 0;
  stack->copy = copy_func;
  stack->free_func = free_func;
  stack->cmp = cmp_func;
  return stack;
}

/**
 * @brief Frees all memory associated with the stack.
 * 
 * All the elements and the stack itself are free.
 *
 * Calls clear() to free each element and then releases internal memory.
 *
 * @param stack Pointer to the Stack.
 */
void free_stack(Stack *stack) {
  if (!stack) {
    return;
  }
  clear(stack);
  free(stack->data);
  free(stack);
}

/**
 * @brief Pushes a new element onto the stack.
 * 
 * Adds a new element to the stack, resizing if necessary.
 * 
 * The element is copied using the user-supplied copy function.
 *
 * @param stack Pointer to the Stack.
 * @param element Pointer to the element to push.
 */
void push(Stack *stack, const void *element) {
  if (!stack) {
    return;
  }
  if (stack->size == stack->capacity) {
    grow(stack);
  }
  stack->data[stack->size++] = stack->copy(element);
}

/**
 * @brief Removes and returns the top element from the stack.
 * 
 * Caller must free the returned element using free_func().
 *
 * @param stack Pointer to the Stack.
 *
 * @return Pointer to the removed element. Caller must be free it. Returns NULL
 * if the stack is empty.
 */
void *pop(Stack *stack) {
  if (!stack) {
    return NULL;
  }
  if (stack->size == 0) {
    return NULL;
  }
  void *element = stack->data[--stack->size];
  stack->data[stack->size] = NULL;
  return element;
}

/**
 * @brief Returns the top element of the stack without removing it.
 *
 * @param stack Pointer to the Stack.
 *
 * @return Pointer to the top element (do not modify or free). Returns NULL if
 * the stack is empty.
 */
void *peek(const Stack *stack) {
  if (!stack) {
    return NULL;
  }
  if (stack->size == 0) {
    return NULL;
  }
  return stack->data[stack->size - 1];
}

/**
 * @brief Clears all elements from the stack.
 *
 * Calls the user-supplied free_func for each element.
 *
 * @param stack Pointer to the Stack.
 */
void clear(Stack *stack) {
  if (!stack) {
    return;
  }
  for (size_t i = 0; i < stack->size; ++i) {
    stack->free_func(stack->data[i]);
  }
  stack->size = 0;
}

/**
 * @brief Checks if the stack has no elements.
 *
 * @param stack Pointer to the Stack.
 *
 * @return true if empty, false otherwise.
 */
bool is_empty(const Stack *stack) {
  if (!stack) {
    return true;
  }
  return stack->size == 0;
}

/**
 * @brief Returns the number of elements in the stack.
 *
 * @param stack Pointer to the Stack.
 *
 * @return Number of elements.
 */
size_t size(const Stack *stack) {
  if (!stack) {
    return 0;
  }
  return stack->size;
}

/**
 * @brief Returns the allocated capacity of the stack.
 *
 * @param stack Pointer to the Stack.
 *
 * @return Current allocated capacity.
 */
size_t capacity(const Stack *stack) {
  if (!stack) {
    return 0;
  }
  return stack->capacity;
}

/**
 * @brief Checks if the stack contains a given element.
 *
 * Requires cmp_func to be non-NULL.
 *
 * @param stack Pointer to the Stack.
 * @param element Pointer to the element to search for.
 *
 * @return true if found, false otherwise.
 */
bool contains(const Stack *stack, const void *element) {
  if (!stack) {
    return false;
  }
  for (size_t i = 0; i < stack->size; ++i) {
    if (stack->cmp(stack->data[i], element) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Creates a deep copy of the stack.
 * 
 * Elements are copied using the user-supplied copy function.
 *
 * @param stack Pointer to the Stack.
 *
 * @return Pointer to a new Stack with copied elements.
 */
Stack *clone(const Stack *stack) {
  if (!stack) {
    return NULL;
  }
  Stack *clone = new_stack(stack->copy, stack->free_func, stack->cmp);
  for (size_t i = 0; i < stack->size; ++i) {
    push(clone, stack->data[i]);
  }
  return clone;
}

/**
 * @brief Reverses the stack in-place by swapping elements.
 *
 * @param stack Pointer to the Stack.
 */
void reverse(Stack *stack) {
  if (stack->size == 0) {
    return;
  }
  size_t left = 0;
  size_t right = stack->size - 1;
  while (left < right) {
    void *tmp = stack->data[left];
    stack->data[left] = stack->data[right];
    stack->data[right] = tmp;
    ++left;
    --right;
  }
}

/**
 * @brief Converts the stack to a newly heap-allocated array.
 * 
 * Caller must free both the array and its elements.
 *
 * @param stack Pointer to the Stack.
 * @param out_size Optional pointer to receive the array size.
 *
 * @return Pointer to a new array containing all stack elements.
 */
void **to_array(const Stack *stack, size_t *out_size) {
  if (stack->size == 0) {
    if (out_size) {
      *out_size = 0;
    }
    return NULL;
  }
  void **array = malloc(stack->size * sizeof(void *));
  if (!array) {
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < stack->size; ++i) {
    array[i] = stack->copy(stack->data[i]);
  }
  if (out_size) {
    *out_size = stack->size;
  }
  return array;
}
