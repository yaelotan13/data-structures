/**********************************************
			Author: Yael Lotan
			Revewer: Sandra Hadad 
**********************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "./include/dhcp.h" /* dhcp header file */
#include "./include/ip.h" /* ip header file */
#include "./include/trie.h" /* trie header file */

struct trie 
{
	size_t num_of_bits;
	node_t *root;
};

struct node
{
	int is_full;
	node_t *children[2];
};

typedef enum side {LEFT = 0, RIGHT = 1} side_t;
typedef enum boolean {YES = 0, NO = 1} bool_t;
enum availability {FREE = 0, TAKEN = 1, LEAF = 2}; /* notice that leaf is 2 */

static enum status RecurInsert(node_t *current, int index, side_t side, 
							   uint32_t data, uint32_t *result_add);
static int BitIsOn(uint32_t arr, int index);
uint32_t BitFlip(uint32_t arr, int index);
static size_t CountNodes(node_t *root);
static void DestroyAll(node_t *trie);
static node_t *CreateNode();
static void UpdateHeights(node_t *current);

static int BitIsOn(uint32_t arr, int index)
{
	return (0 == (arr & (1lu << (index))) ? 0 : 1); 
}

uint32_t BitFlip(uint32_t arr, int index)
{
	uint32_t mask = 1 << (index);
	
	return (arr ^ mask);
}

static void UpdateHeights(node_t *current) 
{
	if (NULL != current->children[LEFT] &&
		NULL != current->children[RIGHT])
	{
		if (FREE != current->children[LEFT]->is_full &&
			FREE != current->children[RIGHT]->is_full)
		{
			current->is_full = TAKEN;
		}
	}
}

static enum status RecurInsert(node_t *current, int index, side_t side, 
							   uint32_t data, uint32_t *result_add)
{
	enum status status = 0; 

	side = BitIsOn(data, index - 1); /* check to which side to go */

	if (TAKEN == current->is_full)
	{
		return ADDR_TAKEN;
	}

	if (1 == index && NULL == current->children[side]) /* if the you found the place to insert the node and it is empty */
	{
		current->children[side] = CreateNode();
		if (!current->children[side])
		{
			return E_MALLOC;
		}
		current->children[side]->is_full = LEAF;

		UpdateHeights(current);
		
		return OK;
	}
	else if (1 == index && NULL != current->children[side] && 
			 LEAF == current->children[side]->is_full) /* if you found the place but there is already a leaf there */
	{
		return ADDR_TAKEN;
	}
	else if (1 == index && NULL != current->children[side]) /* if you found the place and thei'r is_full is empty */
	{
		current->children[side]->is_full = LEAF;

		UpdateHeights(current);
		
		return OK;
	}

	if (NULL == current->children[side]) /* if you need to create a new node on the path */
	{
		current->children[side] = CreateNode();
		if (!current->children[side])
		{
			return E_MALLOC;
		}
	}

	status = RecurInsert(current->children[side], index - 1, side, data, result_add);

	if (ADDR_TAKEN == status) /* call itself again but go to the other direction */
	{
		status = RecurInsert(current, index - 1, !side, data, result_add);
		*result_add = BitFlip(*result_add, index - 1); /* update the result address with the change */
	}

	UpdateHeights(current);

	return status;
}

static size_t CountNodes(node_t *root)
{  
    size_t count = 0;
    
    if (root)
    {
    	if (root->is_full == LEAF) /* count only leaves */
    	{
        	++count;	
    	}
        count += CountNodes(root->children[LEFT]);
        count += CountNodes(root->children[RIGHT]);
    }

    return count;
}

static void DestroyAll(node_t *trie)
{
    if (!trie)
    {
        return;
    }
    
    DestroyAll(trie->children[LEFT]);
    DestroyAll(trie->children[RIGHT]);
    
    free(trie);
    trie = NULL;
}

static node_t *CreateNode()
{
	node_t *node = (node_t *)malloc(sizeof(*node));
	if (!node)
	{
		return NULL;
	}

	node->is_full = FREE;
	node->children[LEFT] = NULL;
	node->children[RIGHT] = NULL;

	return node;
}

trie_t *TrieCreate(size_t num_of_bits)
{
	trie_t *trie = (trie_t *)malloc(sizeof(*trie));
	if (!trie)
	{
		return NULL;
	}
	
	trie->root = CreateNode();
	if (!trie->root)
	{
		free(trie);
		trie = NULL;

		return NULL;
	}

	trie->num_of_bits = num_of_bits;

	return trie;
}

void TrieDestroy(trie_t *trie)
{
	assert (NULL != trie);

	DestroyAll(trie->root);
	free(trie);
	trie = NULL;
}

size_t TrieCount(const trie_t *trie)
{
	assert (NULL != trie);

	return CountNodes(trie->root);
}

enum status TrieInsert(trie_t *trie, uint32_t data, uint32_t *ins_data)
{
	int i = 0;
	size_t side = LEFT;
	enum status result = 0;

	assert (NULL != trie);

	i = trie->num_of_bits;
	*ins_data = data;

	if (trie->root->is_full)
	{
		return NO_FREE_ADDR;
	}

	result = RecurInsert(trie->root, i, side, data, ins_data);
	
	if (OK == result && *ins_data != data)
	{
		return ADDR_TAKEN;
	}

	return result; 
}

int RecurRemove(node_t *current, uint32_t key, int index, side_t side)
{	
	if (LEAF == current->is_full)
	{
		current->is_full = FREE;

		return YES;
	}
	if (FREE == current->is_full && 0 == index)
	{
		return NO;
	}

	--index;
	side = BitIsOn(key, index);
	current->is_full = FREE;

	return RecurRemove(current->children[side], key, index, side);
}

int TrieRemove(trie_t *trie, uint32_t key)
{
	int i = 0;
	size_t side = LEFT;
	int result = 0;

	assert (NULL != trie);

	i = trie->num_of_bits;

	result = RecurRemove(trie->root, key, i, side);

	return result;
}
