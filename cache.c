#include <stdlib.h>
#include <stdio.h>
#include "cache.h"


Node* create_node(char *key) {
    Node* new_node = (Node *)malloc( sizeof( Node ) );
    new_node->key = key;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

CappedQueue* create_capped_queue(unsigned capacity) {
    CappedQueue* queue = (CappedQueue *)malloc( sizeof( CappedQueue ));
    queue->capacity = capacity;
    queue->count = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

HashMap* create_hash_map(unsigned capacity) {
    HashMap* hm = (HashMap *)malloc( sizeof( HashMap ) );
    hm->capacity = capacity;
    hm->buckets = (char **) malloc( hm->capacity * sizeof( char* ) );
    int i;
    for( i = 0; i < hm->capacity; ++i )
        hm->buckets[i] = NULL;
    return hm;
}

unsigned djb_hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c == *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

int queue_is_capped(CappedQueue* cq)
{
    return cq->capacity == cq->count;
}

int queue_is_empty(CappedQueue* cq)
{
    return cq->head == NULL;
}

void enqueue(CappedQueue* cq, char* key)
{
    Node* to_add = create_node(key);
    to_add->next = cq->head;

    if ( queue_is_empty( cq ) )
        cq->tail = cq->head = to_add;
    else 
        cq->head->prev = to_add;
        cq->head = to_add;
}


// int main()
// {
//     CappedQueue* queue = create_capped_queue(10);
//     enqueue(queue, "one");
//     enqueue(queue, "two");
//     // enqueue(queue, "three");

//     struct Node *tmp, *tmpt;
//     tmp = queue->head;
//     tmpt = queue->tail;
//     printf("Data = %s\n", tmp->key);
//     printf("Data = %s\n", tmp->next->key);
//     printf("Data = %s\n", tmpt->key);
//     // while (tmp != NULL)
//     // {
//     //     printf("Data = %s\n", tmp->key);
//     //     tmp = tmp->next;   
//     // }

// }

