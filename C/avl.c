/*************************************
        Author: Yael Lotan
        Reviewr: Eran Segal
*************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <unistd.h> /* ssize_t */

#include "./include/avl.h" /* avl header file */

typedef struct avl_node
{
	void *data;
	struct avl_node *children[2];
	ssize_t height;
} avl_node_t;

 struct avl
{
	avl_node_t *root;
	avl_cmp_f compare;
	const void *params;
};

typedef enum side
{   LEFT = 0,
    RIGHT = 1
} side_t;

static avl_node_t *CreateNode(void *data);
static int HasNoChildren(avl_node_t *node);
static int HasTwoChildren(avl_node_t *node);
static void DeleteChild(avl_node_t *node);
static ssize_t Max(ssize_t left_height, ssize_t right_height);
static int HasOnlyLeftChild(avl_node_t *node);
static void *GetData(avl_node_t *node);
static ssize_t GetHeight(avl_node_t *node);
static int IsBalanced(avl_node_t *node);
static void UpdateHeight(avl_node_t *node);
static avl_node_t *InsertNode(avl_node_t *node, void *data, avl_cmp_f compare, const void *params, int *status);
static avl_node_t *FindNode(avl_node_t *node, void *key, avl_cmp_f compare, const void *params);
static avl_node_t *BalanceTree(avl_node_t *node);
static avl_node_t *RotateLeft(avl_node_t *node);
static avl_node_t *RotateRight(avl_node_t *node);
static int FindAndFreeSucc(avl_node_t *node);
static void RemoveNode(avl_node_t *node, void *key, avl_cmp_f compare, const void *params);
static int ForEach(avl_node_t *node, avl_act_f Act, const void *args);
static size_t CountNodes(avl_node_t *node);
static void DestroyAll(avl_node_t *node);
static int BalanceFactor(avl_node_t *node);
static side_t RotationCase(avl_node_t *node);

avl_t *AVLCreate(avl_cmp_f Compare, const void *params)
{
    avl_t *avl = NULL;
    
    assert(NULL != Compare);
    
    avl = (avl_t *)malloc(sizeof(avl));
    
    if (NULL == avl)
    {
        return NULL;
    }
    
    avl->root = NULL;
    avl->compare = Compare;
    avl->params = params;
    
    return avl;
}

void AVLDestroy(avl_t *avl)
{
    assert(NULL != avl);
    
    DestroyAll(avl->root);
    free(avl);
}

size_t AVLGetHeight(const avl_t *avl)
{
    ssize_t left_height = 0;
    ssize_t right_height = 0;
    
    assert(NULL != avl);
    assert(NULL != avl->root);
    
    left_height = GetHeight(avl->root->children[LEFT]);
    right_height = GetHeight(avl->root->children[RIGHT]);
    
    return Max(left_height, right_height) + 1;
}

void *AVLFind(const avl_t *avl, const void *key)
{
    avl_node_t *node = NULL;
    
    assert(NULL != avl);
    
    node = FindNode(avl->root, (void *)key, avl->compare, avl->params);
    
    if (NULL == node)
    {
        return NULL;
    }
    
    return GetData(node); 
}

void AVLRemove(avl_t *avl, const void *key)
{
	if (avl->root->data == key)
	{
		free(avl->root);
		avl->root = NULL;
	
		return;
	}
	
	RemoveNode(avl->root, (void *)key, avl->compare, avl->params);
	
    if (!IsBalanced(avl->root))
	{
	    avl->root = BalanceTree(avl->root);
	}
}

int AVLIsEmpty(const avl_t *avl)
{
    assert(NULL != avl);
    
    return avl->root == NULL;
}

int AVLForEach(avl_t *avl, avl_act_f Act, const void *args)
{
    assert(NULL != avl);
    assert(NULL != Act);
    
    return ForEach(avl->root, Act, args);
}

int AVLInsert(avl_t *avl, void *data)
{
    int status = 0;

	avl->root = InsertNode(avl->root, data, avl->compare, avl->params, &status);
	
    return status;
}

size_t AVLGetSize(const avl_t *avl)
{
    size_t count = 0;
    
    assert(NULL != avl);
    
    count = CountNodes(avl->root);
    
    return count;
}

static avl_node_t *CreateNode(void *data)
{
    avl_node_t *new_avl = (avl_node_t *)malloc(sizeof(*new_avl));
    
    if (NULL == new_avl)
    {
        return NULL;
    }
    
    new_avl->data = data;
    new_avl->children[RIGHT] = NULL;
    new_avl->children[LEFT] = NULL;
    new_avl->height = 0;
    
    return new_avl;
}

static int HasNoChildren(avl_node_t *node)
{
	return NULL == node->children[LEFT] && NULL == node->children[RIGHT];
}

static int HasTwoChildren(avl_node_t *node)
{
	return NULL != node->children[LEFT] && NULL != node->children[RIGHT];
}

static void DeleteChild(avl_node_t *node)
{
    free(node);
    node = NULL;
}

static ssize_t Max(ssize_t left_height, ssize_t right_height)
{
    return left_height > right_height ? left_height : right_height;
}	

static int HasOnlyLeftChild(avl_node_t *node)
{
    return NULL != node->children[LEFT]; 
}

static void *GetData(avl_node_t *node)
{
    return node->data;
}

static ssize_t GetHeight(avl_node_t *node)
{
    if (NULL == node)
    {
        return -1;
    }
    
    return node->height;
}

static int IsBalanced(avl_node_t *node)
{
    ssize_t balance_factor = BalanceFactor(node);
    
    if (-1 > balance_factor || 1 < balance_factor)
    {
        return 0;
    }
    
    return 1;
}

static int BalanceFactor(avl_node_t *node)
{
    ssize_t left_height = GetHeight(node->children[LEFT]);
    ssize_t right_height = GetHeight(node->children[RIGHT]);
    
    return left_height - right_height;
}

static void UpdateHeight(avl_node_t *node)
{
    ssize_t left_height = GetHeight(node->children[LEFT]);
    ssize_t right_height = GetHeight(node->children[RIGHT]);
    
    node->height = Max(left_height, right_height) + 1;
}

static avl_node_t *InsertNode(avl_node_t *node, void *data, avl_cmp_f compare, const void *params, int *status)
{
	avl_node_t *new_node = NULL;
	side_t next_side = LEFT;
    
    if (NULL == node)
	{
		new_node = CreateNode(data);
		
		if (NULL == new_node)
		{
		    *status = 1;
		}
		
		return new_node;
	}
	
	if (0 > compare(node->data, data, params))
	{
		next_side = RIGHT;
	}
	
    node->children[next_side] = InsertNode(node->children[next_side], data, compare, params, status);
	UpdateHeight(node);
	
	if (!IsBalanced(node))
	{
	    node = BalanceTree(node);
	}
	
	return node; 
}

static avl_node_t *BalanceTree(avl_node_t *node)
{
    ssize_t balance_factor = 0;

    if (LEFT == RotationCase(node)) /* left case */
    {
        balance_factor = BalanceFactor(node->children[LEFT]);
        
        if (0 > balance_factor) /* left right case */
        {
            node->children[LEFT] = RotateLeft(node->children[LEFT]);
        }
        
        node = RotateRight(node); 
    }
    else /* right case */
    {
        balance_factor = BalanceFactor(node->children[RIGHT]);
        
        if (0 < balance_factor) /* right left case */
        {
            node->children[RIGHT] = RotateRight(node->children[RIGHT]);
        }
        
        node = RotateLeft(node); 
    }
    
    UpdateHeight(node);
    
    return node;
}

static avl_node_t *FindNode(avl_node_t *node, void *key, avl_cmp_f compare, const void *params)
{
	side_t next_side = LEFT;
	int cmp_result = compare(node->data, key, params);
    
	if (0 > cmp_result)
	{
		next_side = RIGHT;
	}
	else if (0 < cmp_result)
	{
		next_side = LEFT;
	}
	else
	{
		return node;
	}
	
	if (NULL == node->children[next_side])
    {
        return NULL;
    }
	
	return FindNode(node->children[next_side], key, compare, params);
}

static avl_node_t *RotateLeft(avl_node_t *root)
{
    avl_node_t *pivot = root->children[RIGHT];
    
    root->children[RIGHT] = pivot->children[LEFT];
    pivot->children[LEFT] = root;
    
    UpdateHeight(root);
    
    return pivot;
}

static avl_node_t *RotateRight(avl_node_t *root)
{
    avl_node_t *pivot = root->children[LEFT];
    
    root->children[LEFT] = pivot->children[RIGHT];
    pivot->children[RIGHT] = root;
    
    UpdateHeight(root);
    
    return pivot;
}

static enum side RotationCase(avl_node_t *node)
{
    ssize_t left_height = GetHeight(node->children[LEFT]);
    ssize_t right_height = GetHeight(node->children[RIGHT]);
    
    return left_height > right_height ? LEFT : RIGHT;
}

static int FindAndFreeSucc(avl_node_t *node)
{
	int *data = 0;

	if (NULL == node->children[LEFT]->children[LEFT])
	{
		data = node->children[LEFT]->data;
		free(node->children[LEFT]);
		node->children[LEFT] = NULL;

		return *data;
	}
	
	return FindAndFreeSucc(node->children[LEFT]);
}

static void RemoveNode(avl_node_t *node, void *key, avl_cmp_f compare, const void *params)
{
	side_t side = LEFT;
	int cmp_result = compare(node->data, key, params);
	int *succ_data = NULL;
	avl_node_t *children_node = NULL;
	avl_node_t *right_node_children = NULL;

	if (0 > cmp_result)
	{
		side = RIGHT;
	}
	if (0 < cmp_result)
	{
		side = LEFT;
	}
	if (NULL == node->children[side])
	{
	    return;
	}
	
	children_node = node->children[side];
	
	if (0 == compare(children_node->data, key, params))
	{
		if (HasNoChildren(children_node)) /* has no children */
		{
            DeleteChild(children_node);
            node->children[side] = NULL;
            UpdateHeight(node);
            
            return;
		}
		if (HasTwoChildren(children_node)) /* has two children */
		{
			if (NULL == children_node->children[RIGHT]->children[LEFT]) /* successor is the right child */
            {
                right_node_children = children_node->children[RIGHT]->children[RIGHT];
                side = RIGHT;
            }
			else /* find successor, get it's data and free it */
			{
				*succ_data = FindAndFreeSucc(children_node->children[RIGHT]);
			}
		}
		else /* has one child */
		{
		    if (HasOnlyLeftChild(children_node))
		    {
		        side = LEFT;
		    }
		    else
		    {
		        side = RIGHT;
		    }
		}
		
        succ_data = GetData(children_node->children[side]);
        DeleteChild(children_node->children[side]);
        children_node->children[RIGHT] = right_node_children;
        children_node->children[side] = NULL;
        children_node->data = succ_data;
        UpdateHeight(node);
		        
        return;
	}
	
	RemoveNode(node->children[side], key, compare, params);
	UpdateHeight(node);
	
	if (!IsBalanced(node))
	{
	    BalanceTree(node);
	}
}

static int ForEach(avl_node_t *node, avl_act_f Act, const void *args)
{
    int status = 0;
    
    if (0 == status && node->children[LEFT])
    {
        status = ForEach(node->children[LEFT], Act, args);
    }
    
    status = Act(node->data, args);
    
    if (0 == status && node->children[RIGHT])
    {
        status = ForEach(node->children[RIGHT], Act, args);
    }
    
    return status;
}

static size_t CountNodes(avl_node_t *node)
{  
    size_t count = 0;
    
    if (node)
    {
        ++count;
        count += CountNodes(node->children[LEFT]);
        count += CountNodes(node->children[RIGHT]);
    }
    
    return count;
}

static void DestroyAll(avl_node_t *node)
{
    if (NULL == node)
    {
        return;
    }
    
    DestroyAll(node->children[LEFT]);
    DestroyAll(node->children[RIGHT]);
    
    free(node);
    node = NULL;
}
