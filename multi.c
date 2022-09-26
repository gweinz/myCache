#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "cache.h"
#include <unistd.h>

typedef struct{
    char *k;
    CappedQueue *q;
    } args;

void print_queue(CappedQueue* queue) {
    struct Node *tmp;
    tmp = queue->head;
    while (tmp != NULL)
    {
        printf("Node = %s\n", tmp->key);
        tmp = tmp->next;   
    }
}

void *some_func (void* arg)
{
    args *argsa = (args*)arg;
    sleep (1); //to give other threads chances to cut in
    enqueue((*argsa).q, (*argsa).k);
    pthread_exit( NULL );
}
int main()
{
    CappedQueue* queue = create_capped_queue(10);
    pthread_t thread[3];
    char* alph[10] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
    int index;
    args *arg;

    for (index = 0; index < 3; index++)
    {
        arg = malloc(sizeof(args));
        (*arg).k = alph[index];
        (*arg).q = queue;
        // tmp->k=alph[index];
        // tmp->q=queue;
        
        if (pthread_create(&thread[index], NULL, some_func, arg ))
        {
            printf ("something went wrong creating the thread"); 
        }

    }
    pthread_join ( thread[0], NULL);
    pthread_join ( thread[1], NULL);
    pthread_join ( thread[2], NULL);

    print_queue(queue);
    printf("q size %d \n", queue->count);
    return 0;
}