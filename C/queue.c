/*****************************************
		Author: Yael Lotan
		Reviewer: Daniel Maizel
*****************************************/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "./include/queue.h" /* header file */
#include "./include/sll.h" /* header file of sll*/

struct queue
{
	sll_t *list;
};

queue_t *QueueCreate()
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    if (NULL == queue)
    {
        return NULL;
    }
    
    queue->list = SLLCreate();
    
    return queue;
}

void QueueDestroy(queue_t *queue)
{
    SLLDestroy(queue->list);
    free(queue);
}

size_t QueueSize(const queue_t *queue)
{
    size_t result = SLLSize(queue->list);
    
    return result;
}

void *QueuePeek(const queue_t *queue)
{
    it_t last = NULL;
    void *result = NULL;
    last = SLLBegin(queue->list);
    result = SLLGetData(last);
    
    return result;
}

int QueueEnqueue(queue_t *queue, const void *data)
{
    it_t first = NULL;
    it_t result_push = SLLPushBack(queue->list, data);
    
    return first != result_push;
}

void QueueDequeue(queue_t *queue)
{
    SLLPopFront(queue->list);
}


int QueueIsEmpty(const queue_t *queue)
{
    int result = SLLIsEmpty(queue->list);
   
    return result;
}

queue_t *QueueAppend(queue_t *dest, queue_t *src) 
{
    it_t runner_src = SLLBegin(src->list);
    void *data = NULL;
    
    while (SLLEnd(src->list) != SLLNext(runner_src))
    {
        data = SLLGetData(runner_src);
        QueueEnqueue(dest, data);
        runner_src = SLLNext(runner_src);
    }
     
    return dest;
}
