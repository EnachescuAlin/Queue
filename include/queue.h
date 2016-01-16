#ifndef PHILIB_QUEUE_H
#define PHILIB_QUEUE_H

#define QUEUE_FRONT_POP_PROCESSING 1
#define QUEUE_FRONT_PROCESSING_POP 2

typedef struct Queue*   QUEUE;
typedef int             QUEUE_CODE;
typedef void*           QUEUE_ITEM;
typedef int             QUEUE_PROCESSING_TYPE;

typedef void        (*queue_processesItemFn)(QUEUE_ITEM);
typedef void        (*queue_freeItemFn)(QUEUE_ITEM);
typedef QUEUE_ITEM  (*queue_copyItemFn)(const QUEUE_ITEM);

struct Queue
{
    struct QueueNode
    {
        struct QueueNode *next;
        QUEUE_ITEM        item;
    } *first;
    queue_freeItemFn free;
    queue_copyItemFn copy;
};

/*
 * errors codes
 */
#define QUEUE_NULL_POINTER              0
#define QUEUE_NOT_EMPTY                 0
#define QUEUE_EMPTY                     1
#define QUEUE_NO_ERROR                  2
#define QUEUE_MALLOC_ERROR              3
#define QUEUE_INVALID_PROCESSING_TYPE   4

#ifdef __cplusplus
extern "C" {
#endif

QUEUE       queue_init(queue_freeItemFn, queue_copyItemFn);
QUEUE       queue_copy(const QUEUE);
QUEUE_CODE  queue_delete(QUEUE*);
QUEUE_CODE  queue_empty(const QUEUE);
QUEUE_CODE  queue_reverse(QUEUE);
QUEUE_CODE  queue_push(QUEUE, QUEUE_ITEM);
QUEUE_CODE  queue_pop(QUEUE);
QUEUE_CODE  queue_for_each(QUEUE, QUEUE_PROCESSING_TYPE, queue_processesItemFn);
QUEUE_ITEM  queue_front(const QUEUE);
QUEUE_ITEM  queue_back(const QUEUE);

#ifdef __cplusplus
}
#endif

#define QUEUE_FOR_EACH(queue, type, func, ...)              \
    do                                                      \
    {                                                       \
        if (type == QUEUE_FRONT_PROCESSING_POP)             \
        {                                                   \
            while (queue_empty(queue) == QUEUE_NOT_EMPTY)   \
            {                                               \
                func(queue_front(queue), ##__VA_ARGS__);    \
                queue_pop(queue);                           \
            }                                               \
        }                                                   \
        if (type == QUEUE_FRONT_POP_PROCESSING)             \
        {                                                   \
            queue_freeItemFn fn = queue->free;              \
            queue->free = NULL;                             \
            while (queue_empty(queue) == QUEUE_NOT_EMPTY)   \
            {                                               \
                QUEUE_ITEM item = queue_front(queue);       \
                queue_pop(queue);                           \
                func(item, ##__VA_ARGS__);                  \
                if (fn)                                     \
                    fn(item);                               \
            }                                               \
            queue->free = fn;                               \
        }                                                   \
    }                                                       \
    while (0)

#endif

