/****************************************************
            Author: Yael Lotan
            Reviewer: Andrei Plakhota
****************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "./include/bst.h" /* bst header file */

struct bst_node
{
	void *data;
	bst_it_t parent;
	bst_it_t children[2];
};

struct bst
{
	struct bst_node dummy;
	cmp_f compare;
	const void *params;
};

enum side
{   LEFT = 0,
    RIGHT = 1
};

static bst_it_t GetMinValue(const bst_it_t node)
{
    bst_it_t runner = node;
    
    while (NULL != runner->children[LEFT])
    {
        runner = runner->children[LEFT];
    }
    
    return runner;
}

static bst_it_t GetMaxValue(const bst_it_t node)
{
    bst_it_t runner = node;
    
    while (NULL != runner->children[RIGHT])
    {
        runner = runner->children[RIGHT];
    }
    
    return runner;
}

static int HasChildren(const bst_it_t node)
{
    return NULL != node->children[RIGHT] || NULL != node->children[LEFT];
}

static int HasTwoChildrens(const bst_it_t node)
{
    return NULL != node->children[RIGHT] && NULL != node->children[LEFT];
}

static int HasLeftChildren(const bst_it_t node)
{
    return NULL != node->children[LEFT];
}

static int IsLeftChildren(const bst_it_t node)
{
    bst_it_t parent = node->parent;
    
    return node == parent->children[LEFT];
}

static bst_it_t CreateNode(void *data, bst_it_t parent)
{
    bst_it_t new_node = (bst_it_t)malloc(sizeof(*new_node));
    if (NULL == new_node)
    {
        return NULL;
    }
    
    new_node->data = data;
    new_node->parent = parent;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    
    return new_node;
}

bst_t *BSTCreate(cmp_f compare, const void *params)
{
    bst_t *bst = NULL;
    
    assert(NULL != compare);
    
    bst = (bst_t *)malloc(sizeof(bst_t));
    if (NULL == bst)
    {
        return NULL;
    }
    
    bst->dummy.data = (void *)0xDEADBEEF;
    bst->dummy.parent = NULL;
    bst->dummy.children[LEFT] = NULL;
    bst->dummy.children[RIGHT] = NULL;
    bst->compare = compare;
    bst->params = (void *)params;
    
    return bst;
}

int BSTIsEmpty(const bst_t *bst)
{
    assert(NULL != bst);
    
    return bst->dummy.children[LEFT] == NULL;
}

void *BSTGetData(bst_it_t node)
{
    assert(NULL != node);
    
    return node->data;
}

bst_it_t BSTEnd(const bst_t *bst)
{
    assert(NULL != bst);
    
    return (bst_it_t)(&bst->dummy);
}

bst_it_t BSTBegin(const bst_t *bst)
{
    bst_it_t runner = NULL;
    
    assert(NULL != bst);
    
    runner = (bst_it_t)(&bst->dummy);
    
    while (NULL != runner->children[LEFT])
    {
        runner = runner->children[LEFT];
    }
    
    return runner;
}

bst_it_t BSTInsert(bst_t *bst, void *data)
{
    bst_it_t new_node = NULL;
    bst_it_t runner = NULL;
    bst_it_t parent_runner = NULL;
    int is_left = 0;
    
    assert(NULL != bst);
    
    runner = (bst_it_t)(&bst->dummy);
    parent_runner = (bst_it_t)(&bst->dummy);
    
    if (BSTIsEmpty(bst))
    {
        new_node = CreateNode(data, (bst_it_t)(&bst->dummy));
        runner->children[LEFT] = new_node;
        
        return new_node;
    }
    
    runner = runner->children[LEFT];
    
    while (NULL != runner)
    {
        parent_runner = runner;
        
        if (1 == bst->compare(runner->data, data, bst->params))
        {
            is_left = 1;
            runner = runner->children[LEFT];
        }
        else
        {
            is_left = 0;
            runner = runner->children[RIGHT];            
        }
    }
    
    new_node = CreateNode(data, parent_runner);
    
    if (is_left)
    {
       parent_runner->children[LEFT] = new_node;
       
       return new_node; 
    }
    
    parent_runner->children[RIGHT] = new_node;
    
    return new_node;
}

bst_it_t BSTNext(bst_it_t node)
{
    bst_it_t parent_runner = NULL;
    
    assert(NULL != node);
    
    if (NULL != node->children[RIGHT])
    {
        return GetMinValue(node->children[RIGHT]);
    }
    
    parent_runner = node->parent;
    
    while (node == parent_runner->children[RIGHT])
    {
        node = parent_runner;
        parent_runner = parent_runner->parent;
    }
    
    return parent_runner;
}

bst_it_t BSTPrev(bst_it_t node)
{
    bst_it_t parent_runner = NULL;
    
    assert(NULL != node);
    
    if (NULL != node->children[LEFT])
    {
        return GetMaxValue(node->children[LEFT]);
    }
    
    parent_runner = node->parent;
    
    while (node == parent_runner->children[LEFT])
    {
        node = parent_runner;
        parent_runner = parent_runner->parent;
    }
    
    return parent_runner;
}

int BSTForEach(bst_it_t from, bst_it_t to, act_f action, const void *args)
{
    int status = 0;
    
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != args);
    assert(NULL != action);
    
    while (to != from && 0 == status)
    {
        status = action(from->data, args);
        from = BSTNext(from);
    }
    
    return status;
}

bst_it_t BSTFind(const bst_t *bst, const void *data)
{
	int cmp_result = 0;
	bst_it_t node = (bst_it_t)(&bst->dummy);
	node = node->children[LEFT];
	
	while (NULL != node)
	{	
		cmp_result = bst->compare(BSTGetData(node), data, bst->params);
		if (0 == cmp_result)
		{
			return node;
		}
		node = 0 < cmp_result ? node->children[LEFT] : node->children[RIGHT];
	}

	return BSTEnd(bst);
}

size_t BSTCount(const bst_t *bst)
{
    size_t counter = 0;
    bst_it_t runner = NULL;
    
    assert(NULL != bst);
    
    runner = BSTBegin(bst);
    
    while (BSTEnd(bst) != runner)
    {
        ++counter;
        runner = BSTNext(runner);
    }
    
    return counter;
}

void BSTDestroy(bst_t *bst)
{
    bst_it_t runner = NULL;
    bst_it_t node_to_remove = NULL;
    
    assert(NULL != bst);
    
    runner = (bst_it_t)(&bst->dummy);
    
    while (!BSTIsEmpty(bst))
    {
        if (NULL != runner->children[LEFT])
        {
            runner = runner->children[LEFT]; 
        }
        
        else if (NULL != runner->children[RIGHT])
        {
            runner = runner->children[RIGHT]; 
        }
        else
        {
            if (IsLeftChildren(runner))
            {
                runner->parent->children[LEFT] = NULL;
            }
            else
            {
                runner->parent->children[RIGHT] = NULL;
            }
            
            node_to_remove = runner;
            runner = runner->parent;
            free(node_to_remove);
        }
     }
     
     free(bst);
}

static void DeleteNodeHasNoChilds(bst_it_t node, bst_it_t parent)
{
    if (IsLeftChildren(node))
    {
        parent->children[LEFT] = NULL;
    }
    else
    {
        parent->children[RIGHT] = NULL;
    }

    free(node);
}

static void DeleteNodeHasLeftChild(bst_it_t node, bst_it_t parent)
{
    if (IsLeftChildren(node))
    {
        parent->children[LEFT] = node->children[LEFT];

    }
    else
    {
        parent->children[RIGHT] = node->children[LEFT];
        
    }

    node->children[LEFT]->parent = parent;
    free(node);
}

void BSTRemove(bst_it_t node)
{
    bst_it_t parent = NULL;
    bst_it_t next = NULL;
    
    assert(NULL != node);
    
    if (HasTwoChildrens(node))
    {
        next = BSTNext(node);
        node->data = BSTGetData(next);
        node = next;
    }

    parent = node->parent;
        
    if (!HasChildren(node))
    {
        DeleteNodeHasNoChilds(node, parent);
    }
    else 
    {
        if (HasLeftChildren(node))
        {
            DeleteNodeHasLeftChild(node, parent);
            
            return;
        }
        
       if (IsLeftChildren(node))
        {
            parent->children[LEFT] = node->children[RIGHT];

        }
        else
        {
            parent->children[RIGHT] = node->children[RIGHT];
        }

        node->children[RIGHT]->parent = parent;
        free(node);
    }
}



