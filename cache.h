#ifndef CACHE_H_
#define CACHE_H_

typedef struct Node {
    struct Node *prev, *next;
    char *key;
} Node;

typedef struct CappedQueue {
    Node *head, *tail;
    unsigned capacity;
    unsigned count;
} CappedQueue;

typedef struct HashMap {
    Node **buckets;
    unsigned capacity;
} HashMap;


CappedQueue* create_capped_queue(unsigned capacity);

HashMap* create_hash_map(unsigned capacity);

unsigned djb_hash(char *str);

int queue_is_capped(CappedQueue* cq);

void enqueue(CappedQueue* cq, char* key);

void dequeue(CappedQueue* cq);

void move_to_head(CappedQueue* cq,  Node* item);

void handle_miss(CappedQueue* cq, HashMap* hm, char* key, unsigned hash);

Node* operate(CappedQueue* cq, HashMap* hm, char* key);

#endif // CACHE_H_