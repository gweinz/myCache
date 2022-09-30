#include "cache.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


Node* create_node(char *key) 
{
    Node* new_node = (Node *)malloc( sizeof( Node ) );
    new_node->key = key;
    new_node->val = rand(); 
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

CappedQueue* create_capped_queue(unsigned capacity) 
{
    CappedQueue* queue = (CappedQueue *)malloc( sizeof( CappedQueue ));
    queue->capacity = capacity;
    queue->count = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

HashMap* create_hash_map(unsigned capacity) 
{
    HashMap* hm = (HashMap *)malloc( sizeof( HashMap ) );
    hm->capacity = capacity;
    hm->buckets = (Node **) malloc( hm->capacity * sizeof( Node* ) );
    int i;
    for( i = 0; i < hm->capacity; ++i )
        hm->buckets[i] = NULL;
    return hm;
}

unsigned djb_hash(char *str) 
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

void enqueue(CappedQueue* cq, char* key) {
    Node* to_add = create_node(key);
    to_add->next = cq->head;

    if ( queue_is_empty( cq ) ) {
        cq->tail = cq->head = to_add;
    }
    else {
        cq->head->prev = to_add;
        cq->head = to_add;
    }
    cq->count++;
}

void dequeue(CappedQueue* cq) 
{
    // If empty dont do anything
    if ( queue_is_empty( cq ) ) return;

    if (cq->tail == cq->head) {
        cq->head = NULL;
    }

    // Find old tail remove its linkages and free it
    Node* rmv = cq->tail;
    cq->tail = rmv->prev;
 
    if (cq->tail) cq->tail->next = NULL;
 
    free( rmv );

    cq->count--;
}

void move_to_head(CappedQueue* cq, Node* item) 
{
    if (item == cq->tail) {
        cq->tail = item->prev;
        item->prev->next = NULL;
    }
    else {
        item->prev->next = item->next;
    } 
    item->next = cq->head;
    item->prev = NULL;
    item->next->prev = item;
    cq->head = item;
}

void handle_miss(CappedQueue* cq, HashMap* hm, char* key, unsigned hash)
{
    // if not at capacity
    if (cq->count < cq->capacity) {
        enqueue(cq, key); 
        hm->buckets[hash] = cq->head;
    }
    // if at capacity, deque and add new key to head
    else {
        dequeue(cq);
        hm->buckets[hash] = NULL;
        enqueue(cq, key);
        hm->buckets[hash] = cq->head;
    }
}

Node* operate(CappedQueue* cq, HashMap* hm, char* key) 
{
    unsigned hash = djb_hash(key) % cq->capacity;
    Node* hit = hm->buckets[hash];

    if (hit == NULL) {
        handle_miss(cq, hm, key, hash);
        return hm->buckets[hash];
    }
    // hit but not the right key. aka collision
    else if (strcmp(hit->key, key) != 0) {
        // traverse hashmap because it was a false miss
        for(int i = 0; i < hm->capacity; ++i ){
            if (strcmp(hm->buckets[i]->key, key) == 0) {
                move_to_head(cq, hm->buckets[i]);
                return hm->buckets[i];
            }
        }

        // if there is no correct node then miss
        handle_miss(cq, hm, key, hash);
        return hm->buckets[hash];

    }
    else {
         // if key is present in hashmap retrieve and move to head
        if (hit == cq->head) return hit;
        move_to_head(cq, hit);
        return hit;

        // TODO 
        // return hm->buckets[hash]
    }

 
}

