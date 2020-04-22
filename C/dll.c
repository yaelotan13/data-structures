/**************************************
		Author: Yael Lotan
		Reviewer: Sandra Hadad
**************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "./include/dll.h" /* dll header file */

#define UNUSED(x) (void)(x);

typedef struct dll_node node_t; 

struct dll_node             
{
	void *data;
	struct dll_node *next;
	struct dll_node *prev;
};

struct dll
{
	struct dll_node *head;
	struct dll_node *tail;
};

static node_t *CreateNode(const void *data, it_t next_iter, it_t prev_iter)
{
    node_t *node = (node_t *)malloc(sizeof(*node));
    if (NULL == node)
    {
        return NULL;
    }
    
    node->data = (void *)data;
    node->next = next_iter;
    node->prev = prev_iter;
    
    return node;
}

dll_t *DLLCreate()
{
    dll_t *dll = (dll_t *)malloc(sizeof(dll_t));
    if (NULL == dll)
    {
        return NULL;
    }
    
    dll->tail = CreateNode((void *)0xDEADBEEF, NULL, NULL);
    dll->head = CreateNode((void *)0xDEADBEEF, dll->tail, NULL);
    dll->tail->prev = dll->head;
    
    return dll;
}

void DLLDestroy(dll_t *dll)
{
    node_t *current_node = DLLBegin(dll);
    node_t *free_node = NULL;
    
    assert(NULL != dll);
    
    current_node = DLLPrev(current_node);
    
    while (NULL != current_node)
    {
        free_node = current_node;
        current_node = DLLNext(current_node);
        free(free_node);

    }
    
    free(dll);
}

size_t DLLSize(const dll_t *dll)
{
    size_t counter = 0;
    node_t *node_runner = DLLBegin(dll);
    node_runner = DLLPrev(node_runner);
    
    assert(NULL != dll);

    while (DLLEnd(dll) != DLLNext(node_runner))
    {
        ++counter;
        node_runner = DLLNext(node_runner);
    }
    
    return counter;
}

int DLLIsEmpty(const dll_t *dll)
{
    assert(NULL != dll);
    
    return DLLBegin(dll) == DLLEnd(dll);
}

it_t DLLBegin(const dll_t *dll)
{
    assert(NULL != dll);
    
    return dll->head->next;
}

it_t DLLEnd(const dll_t *dll)
{
    assert(NULL != dll);
    
    return dll->tail;
}

it_t DLLNext(it_t iter)
{
    assert(iter != NULL);
    
    return iter->next;
}

it_t DLLPrev(const it_t iter)
{
    assert(iter->prev != NULL);
    
    return iter->prev;
}

int DLLIsSameIter(const it_t iter1,const it_t iter2)
{
    assert(NULL != iter1 && NULL != iter2);
    
    return iter1 == iter2;
}

void *DLLGetData(const it_t iter)
{
    assert(NULL != iter);
    
    return iter->data;
}

it_t DLLInsert(dll_t *dll, it_t iter, void *data)
{
    it_t next = DLLNext(iter);
    node_t *new_node = CreateNode(data, DLLNext(iter), iter);
    if (NULL == new_node)
    {
        return NULL;
    }
    
    assert(NULL != dll && NULL != iter && NULL != data);
   
    next->prev = new_node;
    iter->next = new_node;
    
    return new_node;
}

it_t DLLInsertBefore(dll_t *dll, it_t iter, const void *data)
{
    it_t next_node = DLLNext(iter);
    it_t prev_node = DLLPrev(iter);
   
    node_t *new_node = CreateNode(data, next_node, prev_node);
    
    assert(NULL != dll && NULL != iter && NULL != data);
   
    next_node->prev = new_node;
    prev_node->next = new_node;
    
    return new_node;
}

it_t DLLErase(it_t where)
{
    it_t next_node = DLLNext(where);
    it_t prev_node = DLLPrev(where);
    
    assert(NULL != where);
    
    prev_node->next = next_node;
    next_node->prev = prev_node;
    
    free(where);
    
    return next_node;
}

it_t DLLPushFront(dll_t *dll, void *data)
{
	it_t first = DLLBegin(dll);
    it_t head = DLLPrev(first);
    it_t new_node = CreateNode(data, first, head);
    
    assert(NULL != dll && NULL != data);
    
    head->next = new_node;
    first->prev = new_node;
    
    return new_node;
}

it_t DLLPushBack(dll_t *dll, void *data)
{
    it_t tail = DLLEnd(dll);
    it_t last = DLLPrev(tail);
    it_t new_node = CreateNode(data, tail, last);
    
    assert(NULL != dll && NULL != data);
    
    tail->prev = new_node;
    last->next = new_node;
    
    return new_node;
}

void DLLPopFront(dll_t *dll)
{
	it_t first = DLLBegin(dll);
    it_t head = DLLPrev(first);
    it_t second = DLLNext(first);
    
    assert(NULL != dll);
   
    head->next = second;
    second->prev = head;
    free(first);
}

void DLLPopBack(dll_t *dll)
{
    it_t last = DLLEnd(dll);
    it_t first = DLLPrev(last);
    it_t second = DLLPrev(first);
    
    assert(NULL != dll);
   
    second->next = last;
    last->prev = second;
    free(first);
}

int DLLForEach(it_t from, it_t to, act_f action, const void* params)
{ 
    int status = 0;
    
    assert(NULL != from && NULL != to && NULL != action);
    
    while (to != from && 0 == status)
    {
        status = action(DLLGetData(from), params);
        from = DLLNext(from);
    }
    
    return status;
}

it_t DLLFind(it_t from, it_t to, cmp_f compare, const void *key, const void *params)
{
    int found = 0;
    
    UNUSED(params);
    
    assert(NULL != from && NULL != to && NULL != key && NULL != compare);
    
    while (to != from && !found)
    {
        if ((found = compare(DLLGetData(from), key, params)))
        {
            return from;
        }
        
        from = DLLNext(from);
    }
    
    return to;
}

void DLLSpliceBefore(it_t where, it_t from, it_t to)
{
	it_t from_prev = DLLPrev(from);
	it_t to_prev = DLLPrev(to);
	it_t where_prev = DLLPrev(where);
 
	to_prev->next = where;
	where_prev->next = from;
	to->prev = from_prev;
    from_prev->next = to;
	from->prev = where_prev;
	
	where->prev = to_prev;
	
}

