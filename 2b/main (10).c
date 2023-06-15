#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//очередь на основе массива(massiva)

typedef struct Queue
{
    int* data;  
    int head;   
    int tail;   
    int count;  
    int size;   
}Queue;

Queue* init(size_t size)
{
    Queue* q = (Queue*)malloc(sizeof(Queue));        
    q->data = (int*)calloc(size, sizeof(int));       
    q->head = q->tail = q->count = 0;
    q->size = size;
    return q;
}


void reallocate_memory(Queue* q)
{
    //переписываем при переполнении
    int* temp = (int*)calloc((q->size) * 2, sizeof(int));     
    for (int i = 0; i < q->count; i++)                      
    {
        temp[i] = q->data[i + q->head];
    }
    free(q->data);                                          
    q->data = temp;                                         
    q->head = 0;
    q->tail = q->count - 1;
    q->size *= 2;
}

//void queue_delete(Queue* queue) {
//    Queue* ptr = queue->head, * ptr_prev;
//    while (ptr) {
//        for (int i = 0; i < queue->count; i++) 
//        {
//
//            ptr[i] = ptr[i + 1];
//        }
//    }
//    free(queue);
//}

void queue_put(Queue* q, int a)
{
    if (q->count == 0)
    {
        q->data[q->tail] = a;
        q->count++;
        return;
    }

    if (q->tail == q->size - 1)                             
        reallocate_memory(q);

    q->data[q->tail + 1] = a;
    q->count++;
    q->tail++;
}

int queue_get(Queue* q)
{
    if (q->count == 0)
    {
        return -1;
    }
    int res = q->data[q->head];
    if (q->head == q->tail)                                 
        q->tail++;
    q->head++;
    q->count--;
    return res;
}


void queue_print(Queue* queue) 
{
    if (queue->count == 0)
        return;
    for (int i = 0; i < queue->count; i++)
        printf("%d ", queue->data[i + queue->head]);

    printf("\n");
}

void data_print(Queue* queue)
{
    for (int i = 0; i < queue->size; i++)
    {
        printf("%d ", queue->data[i]);
    }
}



int main()
{
    int test[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    size_t size = 2;
    Queue* q = init(size);
    queue_print(q);
    for (int i = 0; i < sizeof(test) / sizeof(test[0]); i++)
    {
        queue_put(q, test[i]);
        queue_print(q);
    }

    int local_size = q->count;
    for (int i = 0; i < local_size + 1; i++)
    {
        int temp = queue_get(q);
        if (temp != -1)
        {
            printf("(%d)", temp);
            queue_print(q);
        }
    }
}