/*****************************************
		Author: Yael Lotan
		Reviewer: Eran Segal
*****************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "./include/sll.h"/* header file */

typedef struct node node_t; 
 
struct node
{
    void *data;
    struct node *next;
};

struct sll
{
    struct node *head;
    struct node *tail;
};

static node_t *CreateNode(const void *data, it_t next)
{
    node_t *new = NULL;
    new = (node_t *)malloc(sizeof(node_t));
    if (NULL == new)
    {
        return NULL;
    }
    
    new->data = (void *)data;
    new->next = next;
    
    return new;
}

sll_t *SLLCreate()
{
    sll_t *sll = (sll_t *)malloc(sizeof(sll_t));
    if (NULL == sll)
    {
        return NULL;
    }
    
    sll->tail = CreateNode((void *)0xDEADBEEF, NULL);
    sll->head = CreateNode((void *)0xDEADBEEF, sll->tail);
                                     
    return sll;
}

it_t SLLBegin(const sll_t *sll)
{
    assert(NULL != sll);
    
    return sll->head->next;
}

it_t SLLEnd(const sll_t *sll)
{
    assert(NULL != sll);
    
    return sll->tail;
}

it_t SLLNext(it_t iter)
{
    assert(iter->next != NULL);
    
    return iter->next;
}

it_t SLLPushFront(sll_t *sll, const void *data)
{
    assert(NULL != sll && NULL != data);
    
    sll->head->next =  CreateNode(data, sll->head->next);
    
    return sll->head->next;
}

it_t SLLPushBack(sll_t *sll, const void *data)
{
    node_t *new_node = CreateNode(data, sll->tail);
    node_t *find_last_node = sll->head;
    
    assert(NULL != sll && NULL != data);
  
    while (sll->tail != find_last_node->next)
    {
        find_last_node = find_last_node->next;
    }
   
    find_last_node->next = new_node;
    
    return new_node;
}

void SLLPopFront(sll_t *sll)
{
    node_t *node_to_remove = sll->head->next;
    node_t *new_next_node = sll->head->next->next; 
    
    assert(NULL != sll);
   
    sll->head->next = new_next_node;
    free(node_to_remove);
}

void SLLPopBack(sll_t *sll)
{
    node_t *node_to_remove = NULL;
    node_t *new_next_node = sll->head;
    
    assert(NULL != sll);
    
    while (sll->tail != new_next_node->next->next)
    {
        new_next_node = new_next_node->next;
    }
    
    node_to_remove = new_next_node->next;
    new_next_node->next = sll->tail;
    free(node_to_remove);
}

int SLLIsEmpty(const sll_t *sll)
{
    assert(NULL != sll);
    
    return sll->head->next == sll->tail;
}

size_t SLLSize(const sll_t *sll)
{
    size_t counter = 0;
    node_t *node_runner = sll->head;
    
    assert(NULL != sll);

    while (sll->tail != node_runner->next)
    {
        ++counter;
        node_runner = node_runner->next;
    }
    
    return counter;
}

it_t SLLInsertAfter(sll_t *sll, it_t iter, const void *data)
{
    node_t *new_node = CreateNode(data, iter->next);;
    
    assert(NULL != sll && NULL != iter && NULL != data);
   
    iter->next = new_node;
    
    return new_node;
}

void SLLErase(sll_t *sll, it_t where)
{
    node_t *target_node = NULL;
    node_t *node_to_erase = NULL;
    
    assert(NULL != sll && NULL != where);
    
    target_node = sll->head;
    node_to_erase = NULL;
    
    while (where != target_node->next)
    {
        target_node = target_node->next;
    }
    
    node_to_erase = target_node->next;
    target_node->next= target_node->next->next;
    free(node_to_erase);
}

void SLLDestroy(sll_t *sll)
{
    node_t *current_node = sll->head;
    node_t *free_node = NULL;
    
    assert(NULL != sll);
    
    while (NULL != current_node)
    {
        free_node = current_node;
        free(free_node);
        current_node = current_node->next;
    }
    
    free(sll);
}

void *SLLGetData(it_t iter)
{
    assert(NULL != iter);
    
    return iter->data;
}

int SLLForEach(it_t from, it_t to, const void *param, act_f action)
{ 
    int status = 0;
    
    assert(NULL != from && NULL != to && NULL != param && NULL != action);
    
    while (to != from && 0 == status)
    {
        status = action(from->data, param);
        from = from->next;
    }
    
    return status;
}

it_t SLLFind(it_t from, it_t to, const void *key, cmp_f compare)
{
    int found = 0;
    
    assert(NULL != from && NULL != to && NULL != key && NULL != compare);
    
    found = compare(from->data, key);
    while (to != from && !found)
    {
        from = from->next;
        found = compare(from->data, key);
    }
    
    return from;
}

sll_t *SLLFLip(sll_t *sll)
{
    node_t *previos = NULL;
    node_t *next = NULL;
    node_t *current = SLLBegin(sll);
    
    assert(NULL != sll);
    
    current = SLLBegin(sll);
    while (NULL != current)
    {
        next = current->next;
        current->next = previos;
        previos = current;
        current = next;
    }
    
    sll->tail = sll->head;
    sll->head = previos;
    sll->tail->next = NULL;
    
    return sll;
}
