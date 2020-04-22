typedef struct Stack stack_t;
/* Create a new stack with the size of n_element and type of size_of elements.
Return value: the created stack */
stack_t *STACKCreate(size_t n_elements, size_t size_of_element);
/* Remove the last element from the stack */
void STACKPop(stack_t *stack);
/* Add another element to the top of the stack.
Return value: the stack with the new added element at the top */
int STACKPush(stack_t *stack, const void *element);
/* Look at the last element of the stack without removing it.
Return value: the peeked element */
void *STACKPeek(const stack_t *stack);
/* Check the size of current elements in the stack.
Return vlaue: number of elements */
size_t STACKSize(const stack_t *stack);
/* Destory the stack */
void STACKDestroy(stack_t *stack);
