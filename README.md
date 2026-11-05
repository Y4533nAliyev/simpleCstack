All of these are notes I took while reading "Stack Data Structure" Chapter in the Boot.dev C memory management course. 

Would recommend checking out the course :d

# Low Level Stack

We're going to implement a stack and we're going to do it while **manually managing** the memory of generic pointers.

We'll get to have our first deeper exploration of "generics" in C as well as creating a data structure we will later use in our mark-and-sweep garbage collector.

Assignment:
```C
#include <stddef.h>

typedef struct Stack {
  size_t count;
  size_t capacity;
  // this behaves as an array of void* pointers
  void **data;
} stack_t;

stack_t *stack_new(size_t capacity);
```

```C
stack_t *stack_new(size_t capacity) {
  // create a stack and give it a size of stack_t
  stack_t *stack = malloc(sizeof(stack_t));
  if(stack == NULL){
    return NULL;
  }
  // initially because it is empty, we are going to give count 0
  stack->count = 0;
  stack->capacity = capacity; // use the capacity we got from the function
  // give space of capacity for our data 
  stack->data= malloc(stack->capacity * sizeof(void *));
  if(stack->data == NULL){
    free(stack); // if it doesn't work free our stack
    return NULL;
  }
  return stack;
}
```


# Stack Push

Ok, so let's actually store some data instead of just allocating memory for no reason.

## Making Room

As you know, our stack has a `count` and a `capacity`. But what happenes when the `count` is equal to the `capacity`? We need to make room for more data!

We'll take a simple approach: whenever we run out of capacity, we'll double it. That way we don't have to reallocate memory on every push.

*For example:*
![[Pasted image 20261103181344.png|300]]
The `realloc` function is used to resize memory that was previously allocated with `malloc` or `calloc`. It takes a **pointer to the old memory** and the new size, and returns a pointer to the new memory:
```C
void *realloc(void *ptr, size_t size);
```

```C
int *small = malloc(10 * sizeof(int));
int *large = realloc(small, 20 * sizeof(int));
```

Asssignment
```C
#include "snekstack.h"
#include <stdlib.h>
#include <stdio.h>

void stack_push(stack_t *stack, void *obj);
stack_t *stack_new(size_t capacity);


int main(){
  // we create an empty stack and check it's capacity
  stack_t a = *stack_new(1);
  printf("%zu\n", a.capacity);
 
  // we create an object that we are going to put in our array
  int object = 1;
  void *obj = &object;

  // create a pointer for our stack to push
  stack_t *ptra = &a;
  stack_push(ptra, obj);
  stack_push(ptra, obj);
  
  // see the update in capacity
  printf("%zu\n", a.capacity);
}


void stack_push(stack_t *stack, void *obj){
  // if count is equal to capacity, multiply the capacity by 2
  if(stack->count == stack->capacity){
    size_t old_capacity = stack->capacity; // we are going to keep old_capacity for when it cannot allocate memory
    stack->capacity = stack->capacity * 2;
    
    void *new_data = realloc(stack->data, sizeof(void *) * stack->capacity); // reallocate the new capacity for data
    if(new_data == NULL){
      stack->capacity  = old_capacity;
      return;
    }
    stack->data = new_data; // set the data to the new data
  }
  stack->data[stack->count] = obj; // we use the count as index to add our object and iterate to the next
  stack->count++;
}

// creating a new stack
stack_t *stack_new(size_t capacity) {
  // create a stack and give it a size of stack_t
  stack_t *stack = malloc(sizeof(stack_t));
  if(stack == NULL){
    return NULL;
  }
  // initially because it is empty, we are going to give count 0
  stack->count = 0;
  stack->capacity = capacity; // use the capacity we got from the function
  // give space of capacity for our data 
  stack->data= malloc(stack->capacity * sizeof(void *));
  if(stack->data == NULL){
    free(stack); // if it doesn't work free our stack
    return NULL;
  }
  return stack;
}
```

# Stack Pop

```C
void *stack_pop(stack_t *stack){
  if(stack->count == 0){ // check if we have elements inside 
    return NULL;
  }
  stack->count--; //decrement since we are taking it out
  return stack->data[stack->count]; // return the index
}
```

# Stack free

In C, we don't have a lot of abstractions at our disposal. There are no classes, destructors, functors, etc.

So, to make it easier to work with our `Stack`, we're goind to build out own little `free` function that will clean up all the memory that we've allocated for our stack.

```C
void stack_free(stack_t *stack){
  if(stack == NULL){
    return;
  }
  if(stack->data != NULL){
    free(stack->data);
  }
  free(stack);
}
```

# Dangerous Push

Up until now, even though we made our stack with `void *`, you'll notice that we've only stored plain old `int` pointers.

Now we're going to push an `int *` and a regular `int` directly onto the stack. (**Bad Idea**)

```C
void scary_doublepush(stack_t *stack){
  int value = 1337;
  void *ptr = &value;

  stack_push(stack, ptr);
  int *new = malloc(sizeof(int));
  if(new == NULL){
    return;
  }
  
  *new = 1024;
  stack_push(stack, new);
}
```

# Multiple Types

Now, we will look at how you can also push **different types** of value into the same stack.

**C doesn't care what you put in memory, it's byes all the way down.**

Fortunately, C doesn't let you do these conversions automatically, but it still lets you do them.

*When you are working with `void *`, you're working with raw memory addresses, and you can cast them whatever you want.*

```C
void stack_push_multiple_types(stack_t *stack){
  float *new_float = malloc(sizeof(float));
  if(new_float == NULL){
    return;
  }
  *new_float = 3.14;

  stack_push(stack, new_float);

  const char *string = "Sneklang is blazingly slow";
  size_t length = strlen(string);
  char *new_char = malloc(length);

  strcpy(new_char, string);

  stack_push(stack, new_char);
}
```

*Here we allocate float and char to the heap and then push it to our stack with the help of the `stack_push` function*.
*P.S: We also imported the `string.h` library.

