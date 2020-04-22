/*********************************
		Author: Yael Lotan
		Reviewer: 
*********************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "./include/barr.h"

size_t BARRSetBit(size_t arr, int index, int value)
{
	assert(index <= (signed)sizeof(size_t) * 8);
	assert(value == 0 || value == 1);
	
	if (value == 1)
	{
		return BARRSetOn(arr, index);
	}
	else
	{
		return BARRSetOff(arr, index);
	}	
}

size_t BARRSetOn(size_t arr, int index)
{
	return (arr | (1lu << (index)));
}

size_t BARRSetOff(size_t arr, int index)
{
	size_t mask = 1lu << (index);
	
	assert(index <= (signed)sizeof(size_t) * 8);

	return ((arr & mask) ? (arr ^ (mask)) : (arr));
}

size_t BARRFlip(size_t arr, int index)
{
	size_t mask = 1 << (index);
	
	assert(index <= (signed)sizeof(size_t) * 8);
	
	return (arr ^ mask);
}

size_t BARRMirrorLUT(size_t arr)
{
	int i = 0;
	size_t mirrored = 0;
	unsigned char temp = ' ';
	static unsigned char *ptr_mirror_LUT = NULL;
		
	if (ptr_mirror_LUT == NULL)
	{
		ptr_mirror_LUT = (unsigned char *)calloc(256, sizeof(unsigned char));
		if (NULL == ptr_mirror_LUT)
		{
			return 1;
		}
		
		for (; i < 256; ++i)
		{
			ptr_mirror_LUT[i] = (BARRMirror(i) >> 56);
		}
	}
	
	for (i = 0; i < (signed)sizeof(size_t); ++i)
	{
		temp = (unsigned char)arr;
		mirrored = (mirrored << 8) | ptr_mirror_LUT[(unsigned int)temp];
		arr >>= 8;
	}
	
	return mirrored;
}

size_t BARRMirror(size_t arr)
{
	int i = 0;
	size_t temp = 0;
	size_t right_mask = 0x1;
	size_t left_mask = 0;
	
	if (sizeof(arr) == 8)
	{
		left_mask = 0x8000000000000000;
	}
	else 
	{
		left_mask = 0x80000000;
	}

	for (; i < (signed)sizeof(arr) * 8; ++i)
	{
		if (right_mask & arr)
		{
			temp = temp | left_mask;
		}
		
		right_mask <<= 1;
		left_mask >>= 1;
	}
	
	return temp;
} 

int BARRIsOn(size_t arr, int index)
{
	return (0 == (arr & (1lu << (index))) ? 0 : 1); 
}

int BARRIsOff(size_t arr, int index)
{
	size_t mask = (1 << index) - 1;
	
	assert(index <= (signed)sizeof(size_t) * 8);

	return (!(arr & mask));
}

size_t BARRRotateLeft(size_t arr, size_t amount)
{
	int i = 0;
	size_t mask = 0;
	
	if (sizeof(arr) == 8)
	{
		mask = 0x8000000000000000;
	}
	else 
	{
		mask = 0x80000000;
	}
	
	for (; i < (signed)amount; ++i)
	{
		if (arr & mask)
		{
			 arr <<= 1;
			 arr = BARRSetOn(arr, 0);
		}
		else
		{
			 arr <<= 1;
			 arr = BARRSetOff(arr, 0);
		}
	}
	
	return arr;
}

size_t BARRRotateRight(size_t arr, size_t amount)
{
	int i = 0;
	
	assert(amount <= sizeof(size_t) * 8);

	for (; i < (signed)amount; ++i)
	{
	
		if (arr & 1)
		{
			 arr >>= 1;
			 arr = BARRSetOn(arr, sizeof(size_t) * 8 - 1);
		}
		else
		{
			 arr >>= 1;
			 arr = BARRSetOff(arr, sizeof(size_t) * 8 - 1);
		}
	}
	
	return arr;
}

size_t BARRCountOn(size_t arr)
{
	int times = 8 * sizeof(arr), counter = 0, i = 0;
	
	for(; i < times; ++i)
	{
		if(0 != (arr & 0x01))
		{
			++counter;
		}
		
		arr >>= 1;
	}
	
	return counter;
}

size_t BARRCountOnLUT(size_t arr)
{
	int i = 0;
	size_t total = 0;
	unsigned char temp = ' ';
	static unsigned char *ptr_count_on_LUT = NULL;
		
	if (ptr_count_on_LUT == NULL)
	{
		ptr_count_on_LUT = (unsigned char *)calloc(256, sizeof(unsigned char));
		if (NULL == ptr_count_on_LUT)
		{
			return 1;
		}
		
		for (; i < 256; ++i)
		{
			ptr_count_on_LUT[i] = BARRCountOn(i);
		}
	}
	
	for (i = 0; i < (signed)sizeof(size_t); ++i)
	{
		temp = (unsigned char)arr;
		total += ptr_count_on_LUT[(unsigned int)temp];
		arr >>= 8;
	}
	
	return total;
}

size_t BARRCountOff(size_t arr)
{
	size_t mask = 0x1;
	size_t counter = 0;
	int i = 0;
	
	for (; i < (signed)sizeof(arr) * 8; ++i)
	{
		if (!(arr & (mask << i)))
		{
			++counter;
		}
	}
	
	return counter;
}
