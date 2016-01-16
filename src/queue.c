#include <stdlib.h>

#include "queue.h"

QUEUE queue_init(queue_freeItemFn freeItem, queue_copyItemFn copyItem)
{
    QUEUE queue = (QUEUE) malloc(sizeof(struct Queue));
    if (queue == NULL)
        return NULL;

    queue->first = NULL;
    queue->free = freeItem;
    queue->copy = copyItem;

    return queue;
}

QUEUE_CODE queue_delete(QUEUE *queue)
{
    if (queue == NULL)
        return QUEUE_NULL_POINTER;
    if (*queue == NULL)
        return QUEUE_NULL_POINTER;

    while (queue_empty(*queue) == QUEUE_NOT_EMPTY)
        queue_pop(*queue);

    free(*queue);
    *queue = NULL;

    return QUEUE_NO_ERROR;
}

QUEUE_CODE queue_pop(QUEUE queue)
{
    if (queue == NULL)
        return QUEUE_NULL_POINTER;
    if (queue->first == NULL)
        return QUEUE_EMPTY;

    struct QueueNode *node = queue->first;
    queue->first = node->next;

    if (queue->free != NULL)
        queue->free(node->item);

    node->item = NULL;
    free(node);
    node = NULL;

    return QUEUE_NO_ERROR;
}

QUEUE_CODE queue_empty(QUEUE queue)
{
    if (queue == NULL)
        return QUEUE_NULL_POINTER;

    if (queue->first == NULL)
        return QUEUE_EMPTY;
    else
        return QUEUE_NOT_EMPTY;
}

QUEUE_ITEM queue_front(const QUEUE queue)
{
    if (queue == NULL)
        return NULL;
    if (queue->first == NULL)
        return NULL;

    return queue->first->item;
}

QUEUE_ITEM queue_back(const QUEUE queue)
{
    if (queue == NULL)
        return NULL;
    if (queue->first == NULL)
        return NULL;

    struct QueueNode *node = queue->first;
    while (node->next)
        node = node->next;

    return node->item;
}

QUEUE_CODE queue_push(QUEUE queue, QUEUE_ITEM item)
{
    if (queue == NULL)
        return QUEUE_NULL_POINTER;
    if (item == NULL)
        return QUEUE_NULL_POINTER;

    struct QueueNode *node = (struct QueueNode*) malloc(sizeof(struct QueueNode));
    if (node == NULL)
        return QUEUE_MALLOC_ERROR;

    node->next = NULL;
    node->item = item;

    if (queue->first == NULL)
    {
        queue->first = node;
        return QUEUE_NO_ERROR;
    }

    struct QueueNode *qn = queue->first;
    while (qn->next)
        qn = qn->next;

    qn->next = node;

    return QUEUE_NO_ERROR;
}

QUEUE queue_copy(const QUEUE queue)
{
    if (queue == NULL)
        return NULL;
    if (queue->copy == NULL)
        return NULL;

    QUEUE newQueue  = (QUEUE) malloc(sizeof(struct Queue));
    if (newQueue == NULL)
        return NULL;

    newQueue->first = NULL;
    newQueue->free  = queue->free;
    newQueue->copy  = queue->copy;

    if (queue->first)
    {
        newQueue->first = (struct QueueNode*) malloc(sizeof(struct QueueNode));
        if (newQueue->first == NULL)
        {
            queue_delete(&newQueue);
            return NULL;
        }

        newQueue->first->next = NULL;
        newQueue->first->item = queue->copy(queue->first->item);
    }
    else
        return newQueue;

    struct QueueNode *qn  = queue->first->next;
    struct QueueNode *nqn = newQueue->first;

    while (qn)
    {
        nqn->next = (struct QueueNode*) malloc(sizeof(struct QueueNode));
        if (nqn->next == NULL)
        {
            queue_delete(&newQueue);
            return NULL;
        }

        nqn->next->item = queue->copy(qn->item);
        nqn->next->next = NULL;
        nqn = nqn->next;
        qn  = qn->next;
    }

    return newQueue;
}

QUEUE_CODE queue_for_each(QUEUE queue, QUEUE_PROCESSING_TYPE type,
                          queue_processesItemFn func)
{
    if (queue == NULL || func == NULL)
        return QUEUE_NULL_POINTER;
    if (type != QUEUE_FRONT_POP_PROCESSING && type != QUEUE_FRONT_PROCESSING_POP)
        return QUEUE_INVALID_PROCESSING_TYPE;

    if (type == QUEUE_FRONT_PROCESSING_POP)
    {
        while (queue_empty(queue) == QUEUE_NOT_EMPTY)
        {
            func(queue_front(queue));
            queue_pop(queue);
        }
    }
    else
    {
        queue_freeItemFn fn = queue->free;
        queue->free = NULL;

        while (queue_empty(queue) == QUEUE_NOT_EMPTY)
        {
            QUEUE_ITEM item = queue_front(queue);
            queue_pop(queue);
            func(item);

            if (fn)
                fn(item);
        }

        queue->free = fn;
    }

    return QUEUE_NO_ERROR;
}

QUEUE_CODE queue_reverse(QUEUE queue)
{
    if (queue == NULL)
        return QUEUE_NULL_POINTER;
    if (queue->first == NULL)
        return QUEUE_EMPTY;
    if (queue->first->next == NULL)
        return QUEUE_NO_ERROR;

    struct QueueNode *qn = queue->first;
    struct QueueNode *newFirst = NULL;
    struct QueueNode *copy;

    while (qn)
    {
        copy = qn;
        qn = qn->next;
        copy->next = newFirst;
        newFirst = copy;
    }

    queue->first = newFirst;

    return QUEUE_NO_ERROR;
}

