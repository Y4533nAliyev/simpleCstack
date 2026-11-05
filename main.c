#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int main(){
 
  int object = 1;
  void *obj = &object;

  stack_t *ptra = stack_new(1);
  stack_push(ptra, obj);
  stack_push(ptra, obj);
  stack_pop(ptra);
  printf("%zu\n", ptra->capacity);
  scary_doublepush(ptra);
  stack_pop(ptra);
  stack_push_multiple_types(ptra);
  printf("%zu\n", ptra->count);
  stack_free(ptra);
}


void stack_free(stack_t *stack){
  if(stack == NULL){
    return;
  }
  if(stack->data != NULL){
    free(stack->data);
  }
  free(stack);
}

void *stack_pop(stack_t *stack){
  if(stack->count == 0){ // check if we have elements inside 
    return NULL;
  }
  stack->count--; //decrement since we are taking it out
  return stack->data[stack->count]; // return the index
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

void scary_doublepush(stack_t *stack){ // push an int * and an int to our stack
  int value = 1337;
  void *ptr = &value;
  // this is bad practice as we haven't allocated space in the heap

  stack_push(stack, ptr);
  int *new = malloc(sizeof(int));
  if(new == NULL){
    return;
  }
  
  *new = 1024;
  stack_push(stack, new);
}

void stack_push_multiple_types(stack_t *stack){
  float *new_float = malloc(sizeof(float));
  if(new_float == NULL){
    return;
  }
  *new_float = 3.14;

  stack_push(stack, new_float);

  const char *string = "Sneklang is blazingly slow";
  size_t length = strlen(string) + 1;
  char *new_char = malloc(length);

  strcpy(new_char, string);

  stack_push(stack, new_char);
}
