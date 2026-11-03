#include "stack.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
  stack_t a = *stack_new(1);
  void *obj;
  stack_t *ptr = &a;
  stack_push(ptr, obj);
  printf("Capacity: %zu\n", a.capacity);
  printf("Count: %zu\n", a.count);
  stack_push(ptr, obj);
  printf("Capacity: %zu\n", a.capacity);
  printf("Count: %zu\n", a.count);

  stack_pop(ptr);
  printf("After popping\n");
  printf("----------\n");
  printf("Capacity: %zu\n", a.capacity);
  printf("Count: %zu\n", a.count);
}


stack_t *stack_new(size_t capacity){
  stack_t *stack = malloc(sizeof(stack_t));
  if(stack == NULL){
    exit(1);
  }
  stack->count = 0;
  stack->capacity = capacity;
  stack->data = malloc(capacity * sizeof(void *));
  if(stack->data == NULL){
    free(stack);
    exit(1);
  }
  return stack;
}

void stack_push(stack_t *stack, void *obj){
  if(stack->count == stack->capacity){
    size_t old_capacity = stack->capacity;
    stack->capacity *= 2;
    
    void *newdata = realloc(stack->data, sizeof(void *) * stack->capacity);
    if(newdata == NULL){
      stack->capacity = old_capacity;
      exit(1); 
    }
    stack->data = newdata;
  }
  stack->data[stack->count] = obj;
  stack->count++;
}

stack_t *stack_pop(stack_t *stack){
  if(stack->count == 0){
    return NULL;
  }
  stack->count--;
  return stack->data[stack->count];
}
