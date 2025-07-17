/**
 * @file stack.h
 *
 * @brief Generic stack (LIFO - Last In, First Out) implementation in C.
 *
 * Provides a generic, dynamic stack supporting core operations such as push,
 * pop, peek, and utility functions like clone, reverse, and to_array.
 *
 * The stack is implemented as a dynamically resized array of void pointers.
 *
 * Users must provide functions to copy, free, and optionally compare elements.
 *
 * @author trigologiaa
 */

#ifndef STACK_H

#define STACK_H

#include <bits/types/siginfo_t.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @typedef Stack
 *
 * @brief Opaque struct representing a generic stack.
 */
typedef struct Stack Stack;

/**
 * @typedef StackCopyFunc
 *
 * @brief Function pointer type for copying stack elements.
 *
 * @param element Pointer to the element to copy.
 *
 * @return Pointer to a newly allocated copy of the element.
 */
typedef void *(*StackCopyFunc)(const void *element);

/**
 * @typedef StackFreeFunc
 *
 * @brief Function pointer type for freeing stack elements.
 *
 * @param element Pointer to the element to free.
 */
typedef void (*StackFreeFunc)(void *element);

/**
 * @typedef StackCompareFunc
 *
 * @brief Function pointer type for comparing two stack elements.
 *
 * @param a Pointer to the first element.
 * @param b Pointer to the second element.
 *
 * @return 0 if equal, < 0 if a < b, > 0 if a > b.
 */
typedef int (*StackCompareFunc)(const void *a, const void *b);

/**
 * @brief Creates a new stack.
 *
 * @param copy_func Function to copy elements (must not be NULL).
 * @param free_func Function to free elements (must not be NULL).
 * @param cmp_func Function to compare elements (optional, may be NULL).
 *
 * @return Pointer to the new Stack, or NULL on allocation failure.
 */
Stack *new_stack(StackCopyFunc copy_func, StackFreeFunc free_func, StackCompareFunc cmp_func);

/**
 * @brief Frees all memory associated with the stack.
 *
 * Calls the user-provided free_func for each element.
 *
 * @param stack Pointer to the Stack.
 */
void free_stack(Stack *stack);

/**
 * @brief Pushes a new element onto the stack.
 *
 * @param stack Pointer to the Stack.
 * @param element Pointer to the element to push.
 */
void push(Stack *stack, const void *element);

/**
 * @brief Removes and returns the top element from the stack.
 *
 * @param stack Pointer to the Stack.
 *
 * @return Pointer to the removed element. Caller must be free it. Returns NULL
 * if the stack is empty.
 */
void *pop(Stack *stack);

/**
 * @brief Returns the top element of the stack without removing it.
 *
 * @param stack Pointer to the Stack.
 *
 * @return Pointer to the top element (do not modify or free). Returns NULL if
 * the stack is empty.
 */
void *peek(const Stack *stack);

/**
 * @brief Clears all elements from the stack.
 *
 * Calls free_func for each element.
 *
 * @param stack Pointer to the Stack.
 */
void clear(Stack *stack);

/**
 * @brief Checks if the stack is empty.
 *
 * @param stack Pointer to the Stack.
 *
 * @return true if empty, false otherwise.
 */
bool is_empty(const Stack *stack);

/**
 * @brief Returns the number of elements in the stack.
 *
 * @param stack Pointer to the Stack.
 *
 * @return Number of elements.
 */
size_t size(const Stack *stack);

/**
 * @brief Returns the internal capacity of the stack.
 *
 * @param stack Pointer to the Stack.
 *
 * @return Current allocated capacity.
 */
size_t capacity(const Stack *stack);

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
bool contains(const Stack *stack, const void *element);

/**
 * @brief Creates a deep copy of the stack.
 *
 * @param stack Pointer to the Stack.
 *
 * @return Pointer to a new Stack with copied elements.
 */
Stack *clone(const Stack *stack);

/**
 * @brief Reverses the order of elements in the stack.
 *
 * @param stack Pointer to the Stack.
 */
void reverse(Stack *stack);

/**
 * @brief Converts the stack to a newly allocated array.
 *
 * @param stack Pointer to the Stack.
 * @param out_size Optional pointer to receive the array size.
 *
 * @return Pointer to a new array containing all stack elements.
 */
void **to_array(const Stack *stack, size_t *out_size);

#endif
