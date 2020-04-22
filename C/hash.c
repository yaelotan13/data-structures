/**************************************
		Author: Yael Lotan
		Reviewer: Daniel Maizel
**************************************/

#include <stdlib.h> /* malloc */
#include <math.h> /* sqrt */
#include <assert.h> /* assert */

#include "./include/dll.h" /* dll header file */
#include "./include/hash.h" /* hash header file */

#define UNUSED(x) (void)(x);

struct hash
{
	int (*Compare)(const void *data, const void *key);
	size_t (*HashFunction)(void *key);
	size_t range;
	dll_t **table; 
};

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

static int CompareForFind(const void *data, const void *key, const void *params)
{    
    return ((hash_t *)params)->Compare(data, key);
}

static int InitHashTable(dll_t *table[], size_t range)
{
    size_t i = 0;

	for (; i < range; ++i)
	{
		table[i] = DLLCreate();
		if (NULL == table[i])
		{ 
			return 0;
		}
	}
	
	return 1;
}

hash_t *HashCreate(int (*Compare)(const void *data, const void *key),
					size_t (*HashFunction)(void *key),
					size_t range)
{   
	int init_table = 0;
    hash_t *hash = (hash_t *)malloc(sizeof(*hash)); 
    if (NULL == hash)
    {
        return NULL;
    }
    
    assert (NULL != Compare);
    assert (NULL != HashFunction);
    assert (0 < range);
    
    hash->Compare = Compare;
    hash->HashFunction = HashFunction;
    hash->range = range;
    hash->table = (dll_t **)malloc(range * sizeof(dll_t **)); 
    if (NULL == hash->table)
    {
        free(hash);
        
    	return NULL;
    }
    
    init_table = InitHashTable(hash->table, range);
    if (!init_table)
    {
        free(hash->table);
        free(hash);
        
    	return NULL;
    }
    
    return hash;
}

void HashDestroy(hash_t *table)
{
	size_t i = 0;
	
	assert (NULL != table);
	
	for (; i < table->range; ++i)
	{
		DLLDestroy(table->table[i]);
	}
	
	free(table->table);
	free(table);
}

int HashInsert(hash_t *table, void *data)
{
    size_t index = 0;
    it_t new_node =  NULL;

    assert (NULL != table);
    assert (NULL != data);
        
    index = table->HashFunction(data) % table->range;
    new_node =  DLLPushBack(table->table[index], data);

    if (NULL == new_node)
    {
        return 1;
    } 

    return 0;
}

void HashRemove(hash_t *table, void *key)
{
    size_t index = 0; 
	it_t from = NULL;
	it_t to = NULL;
    it_t element_to_remove = NULL;
    
    assert (NULL != table);
    assert (NULL != key);
    
    index = table->HashFunction(key) % table->range; 
	from = table->table[index]->head->next;
	to = table->table[index]->tail;
    element_to_remove = DLLFind(from, to, CompareForFind, key, table);
    
    DLLErase(element_to_remove);
}

void *HashFind(const hash_t *table, void *key)
{
	size_t index = 0; 
	it_t from = NULL;
	it_t to = NULL;
	it_t found = NULL;
	
	assert (NULL != table);
	assert (NULL != key);
	
    index = table->HashFunction(key) % table->range; 
	from = table->table[index]->head->next;
	to = table->table[index]->tail;
	found = DLLFind(from, to, CompareForFind, key, table);
	
	if (found == to)
	{
	    return NULL;
	}
	
	return found->data;
}

int HashForEach(hash_t *table, 
				int (*Action)(void *data, const void *params),
				const void *params)
{
    size_t i = 0;
    int status = 1;
    
    assert (NULL != table);
    assert (NULL != Action);
    
    if (!HashIsEmpty(table))
    {
        for (; i < table->range; ++i)
	    {
	        if (!DLLIsEmpty(table->table[i]))
	        {
	            it_t from = table->table[i]->head->next;
                it_t to = table->table[i]->tail;
                status = DLLForEach(from, to, Action, params);    
	        }
        }
    }
    
    return status;
}

size_t HashSize(const hash_t *table)
{
    size_t size = 0;
    size_t i = 0;
    
    assert (NULL != table);
    
    for (; i < table->range; ++i)
	{
		size += DLLSize(table->table[i]);
	}
	
	return size;
}

int HashIsEmpty(const hash_t *table)
{
    int is_empty = 1;
    size_t i = 0;
    
    assert (NULL != table);
    
    for (; i < table->range; ++i)
	{
		is_empty = DLLIsEmpty(table->table[i]);
		
		if (!is_empty)
		{
		    return is_empty;
		}
	}
	
	return is_empty;
}

double HashLoad(const hash_t *table)
{
    assert (NULL != table);
    
    return (double)HashSize(table)/table->range;
}

double HashSD(const hash_t *table)
{
    double avrage = 0;
    size_t distance = 0;
    size_t size = 0;
    double sum = 0;
    size_t i = 0;
    
    assert (NULL != table);
    
    avrage = (double)HashSize(table) / table->range;
    
    if (!HashIsEmpty(table))
    {
        for (; i < table->range; ++i)
	    {
	        if (!DLLIsEmpty(table->table[i]))
	        {
                size = DLLSize(table->table[i]); 
                distance = size - avrage;
                sum += (double)(distance * distance);
	        }
        }
    }
    
    return sqrt(sum/HashSize(table));   
}
