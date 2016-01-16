#include <stdio.h>
#include <stdlib.h>
#include <queue.h>

struct Int
{
    int x;
};

struct Int* createInt(int x)
{
    struct Int *_int = malloc(sizeof(struct Int));
    _int->x = x;

    return _int;
}

void deleteInt(void *item)
{
    free(item);
}

void* copyInt(void *item)
{
    return createInt(((struct Int*)item)->x);
}

void processesInt(void *item)
{
    printf("%d ", ((struct Int*)item)->x);
}

void _processesInt(void *item, char c)
{
    printf("%d%c ", ((struct Int*)item)->x, c);
}

void __processesInt(void *item, char c, int i)
{
    printf("%d%c%d ", ((struct Int*)item)->x, c, i);
}

int main(void)
{
    QUEUE queue = queue_init(deleteInt, copyInt);
    for (int i = 1; i <= 5; i++)
        queue_push(queue, createInt(i));

    QUEUE queue2 = queue_copy(queue);
    QUEUE queue3 = queue_copy(queue);
    QUEUE queue4 = queue_copy(queue);
    QUEUE queue5 = queue_copy(queue);
    QUEUE queue6 = queue_copy(queue);
    QUEUE queue7 = queue_copy(queue);
    QUEUE queue8 = queue_copy(queue);
    QUEUE queue9 = queue_copy(queue);

    printf("Queue1:\n");
    printf("Front: %d     Back: %d\n", ((struct Int*)queue_front(queue))->x,
                                       ((struct Int*)queue_back(queue))->x);
    queue_pop(queue);
    printf("Front: %d     Back: %d\n", ((struct Int*)queue_front(queue))->x,
                                       ((struct Int*)queue_back(queue))->x);
    printf("\n");

    printf("Queue2: ");
    queue_for_each(queue2, QUEUE_FRONT_PROCESSING_POP, processesInt);
    printf("\n\n");

    printf("Queue3: ");
    queue_reverse(queue3);
    queue_for_each(queue3, QUEUE_FRONT_PROCESSING_POP, processesInt);
    printf("\n\n");

    printf("Queue4: ");
    QUEUE_FOR_EACH(queue4, QUEUE_FRONT_PROCESSING_POP, _processesInt, 'a');
    printf("\n\n");

    printf("Queue5: ");
    QUEUE_FOR_EACH(queue5, QUEUE_FRONT_POP_PROCESSING, _processesInt, 'b');
    printf("\n\n");

    printf("Queue6: ");
    QUEUE_FOR_EACH(queue6, QUEUE_FRONT_PROCESSING_POP, __processesInt, 'c', 1);
    printf("\n\n");

    printf("Queue7: ");
    QUEUE_FOR_EACH(queue7, QUEUE_FRONT_POP_PROCESSING, __processesInt, 'd', 2);
    printf("\n\n");

    printf("Queue8: ");
    queue_reverse(queue8);
    QUEUE_FOR_EACH(queue8, QUEUE_FRONT_PROCESSING_POP, _processesInt, 'e');
    printf("\n\n");

    printf("Queue9: ");
    queue_reverse(queue9);
    QUEUE_FOR_EACH(queue9, QUEUE_FRONT_POP_PROCESSING, _processesInt, 'f');
    printf("\n\n");

    return 0;
}

