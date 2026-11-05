#include <stddef.h>

typedef struct Stack{
  size_t count;
  size_t capacity;
  void **data;
} stack_t;

void scary_doublepush(stack_t *stack);
void stack_push_multiple_types(stack_t *stack);
void stack_free(stack_t *stack);
void *stack_pop(stack_t *stack);
void stack_push(stack_t *stack, void *obj);
stack_t *stack_new(size_t capacity);
