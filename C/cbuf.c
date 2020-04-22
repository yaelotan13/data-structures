#include <stdio.h> 
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <errno.h> /* errno */

#include "./include/cbuf.h"

#define CAPACITY (cbuf->capacity) 
#define SIZE (cbuf->size) 
#define READ_OFFSET (cbuf->read_offset) 
#define BUFF (cbuf->buff) 

struct CBUF
{
	size_t capacity;
	size_t size;
	size_t read_offset;
	void *buff;
};

cbuf_t *CBUFCreate(size_t capacity)
{
	cbuf_t *cbuf = NULL;
	cbuf = (cbuf_t *)malloc(sizeof(cbuf_t));
	if (NULL == cbuf)
	{
		return NULL;
	}
	
	BUFF = (void *)malloc(capacity);
	if (NULL == BUFF)
	{
		free(cbuf);
		cbuf = NULL;
		
		return NULL;
	}
	
	CAPACITY = capacity;
	READ_OFFSET = 0;
	SIZE = 0;
	
	return cbuf;
}

void CBUFDestroy(cbuf_t *cbuf)
{
	free(BUFF);
	free(cbuf);
}

size_t CBUFCapacity(cbuf_t *cbuf)
{
	return CAPACITY;
}

int CBUFIsEmpty(const cbuf_t *cbuf)
{
	return 0 == SIZE;
}

int CBUFIsFUll(const cbuf_t *cbuf)
{
	return READ_OFFSET == SIZE;
}

size_t CBUFFreeSpace(cbuf_t *cbuf)
{
	if (CBUFIsEmpty(cbuf))
	{
		return CAPACITY; 
	}
		
	return CAPACITY - SIZE;
}

size_t CBUFWrite(cbuf_t *cbuf, const void *data, size_t nbytes)
{
	unsigned int i = 0;
	size_t free_space = CBUFFreeSpace(cbuf);
	char *data_runner = (char*)data;

	if (nbytes > free_space)
	{
		nbytes = free_space;
	}
	
	for (; i < nbytes; ++i)
	{
		*((char*)BUFF + ((SIZE + READ_OFFSET) % CAPACITY)) = *data_runner;
		++data_runner;
		++SIZE;
	}
	
	return nbytes;
}

size_t CBUFRead(cbuf_t *cbuf, void *dest, size_t nbytes)
{
	unsigned int i = 0;
	char *dest_runner = (char*)dest;
	
	if (nbytes >= SIZE)
	{
		nbytes = SIZE;
	}

	for (; i < nbytes; ++i)
	{
		*dest_runner = *((char*)BUFF + READ_OFFSET % CAPACITY);
		++dest_runner;
		++READ_OFFSET;
		--SIZE;
	}
	
	return nbytes;
}

