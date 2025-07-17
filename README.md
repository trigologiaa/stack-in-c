# Stack - Generic Stack (LIFO) Implementation in C

A fully featured generic **Stack** (Last In, First Out) implementation in C supporting core operations such as **push**, **pop**, **peek**, and utility functions like **clone**, **reverse**, **clear**, and **to_array**.

This stack is implemented as a dynamically resized array of `void*` pointers, allowing storage of any data type via user-provided element management functions.

---

## Table of Contents

- [Stack - Generic Stack (LIFO) Implementation in C](#stack---generic-stack-lifo-implementation-in-c)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Usage](#usage)
  - [Functions](#functions)
  - [Author](#author)
  - [License](#license)
  - [Contact](#contact)

---

## Features

- **Generic**: stores any data type using `void *` pointers.
- Dynamic resizing with automatic growth when capacity is exceeded.
- Requires user-supplied functions for:
  - Copying elements (`StackCopyFunc`).
  - Freeing elements (`StackFreeFunc`).
  - Optional comparison of elements (`StackCompareFunc`) for searching.
- Core stack operations:
  - `push` — add element to the top.
  - `pop` — remove and return top element.
  - `peek` — access top element without removal.
  - `clear` — remove all elements.
  - `is_empty` — check if stack is empty.
  - `size` and `capacity` — query current element count and allocated capacity.
  - `contains` — check if element exists (requires comparison function).
  - `clone` — deep copy of the entire stack.
  - `reverse` — reverse element order in-place.
  - `to_array` — export stack elements to a newly allocated array.

---

## Usage

Include the headers and link the implementation in your C project. You must provide appropriate functions to copy, free, and optionally compare your element types.

Example of creating a stack for strings:

```c
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Copy function for strings
void *string_copy(const void *element) {
    const char *str = (const char *)element;
    char *copy = malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

// Free function for strings
void string_free(void *element) {
    free(element);
}

// Compare function for strings
int string_cmp(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

int main() {
    Stack *stack = new_stack(string_copy, string_free, string_cmp);

    push(stack, "Hello");
    push(stack, "World");

    printf("Top element: %s\n", (char *)peek(stack));

    while (!is_empty(stack)) {
        char *elem = pop(stack);
        printf("Popped: %s\n", elem);
        string_free(elem);
    }

    free_stack(stack);
    return 0;
}
```

---

## Functions

- `new_stack(copy_func, free_func, cmp_func)` — Create a new stack with user-supplied element management functions.
- `free_stack(stack)` — Frees all memory used by the stack and its elements.
- `push(stack, element)` — Pushes a copy of the element onto the stack.
- `pop(stack)` — Removes and returns the top element (caller must free).
- `peek(stack)` — Returns the top element without removing (do not free).
- `clear(stack)` — Removes all elements and frees them.
- `is_empty(stack)` — Returns `true` if stack is empty.
- `size(stack)` — Returns number of elements.
- `capacity(stack)` — Returns internal allocated capacity.
- `contains(stack, element)` — Returns `true` if element exists (requires compare function).
- `clone(stack)` — Returns a deep copy of the stack.
- `reverse(stack)` — Reverses the stack elements in place.
- `to_array(stack, out_size)` — Returns a newly allocated array copy of elements.

---

## Author

trigologiaa

---

## License

This project is released under the MIT License. You’re free to use, modify, and distribute.

---

## Contact

For questions, suggestions, or contributions: open an issue or contact the author.