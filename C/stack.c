/*********************************
		Author: Yael Lotan
		Reviewer: Eran Segal
*********************************/

#include <stdio.h> 
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "./include/stack.h"

struct Stack  
{
	void *current;
	void *end;
	int size_of_element;
	char head[1];
};

static int STACKIsFull(stack_t *stack)
{
	return ((char*)stack->current >= (char*)stack->end);
}

static int STACKIsEmpty(const stack_t *stack)
{
	return ((char*)stack->current <= (char*)stack->head);
}

void STACKPop(stack_t *stack)
{
	if (!STACKIsEmpty(stack))
	{
		stack->current = (char*)stack->current - stack->size_of_element; 
	}
}

int STACKPush(stack_t *stack, const void *element)
{
	if (STACKIsFull(stack))
	{
		return 1;
	}
	
	memcpy(stack->current, element, stack->size_of_element);
	stack->current = (char*)stack->current + stack->size_of_element;
	
	return 0;
}

void *STACKPeek(const stack_t *stack)
{
	if (STACKIsEmpty(stack))
	{
		return NULL;
	}
	
	return ((char*)stack->current - stack->size_of_element);
}

stack_t *STACKCreate(size_t n_elements, size_t size_of_element)
{
	stack_t *stack = (stack_t*)malloc(sizeof(stack_t) + n_elements * size_of_element - 1);
	if (NULL == stack)
	{
		return NULL;
	}
	
	stack->current = stack->head;
	stack->size_of_element = size_of_element;
	stack->end = (char*)stack->current + n_elements*size_of_element;
	
	assert(size_of_element > 0);
	
	return stack;
}

size_t STACKSize(const stack_t *stack)
{
	return (((char*)stack->current - (char*)stack->head)/ stack->size_of_element);
}

void STACKDestroy(stack_t *stack)
{
	free(stack);
}
