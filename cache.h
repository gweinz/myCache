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
    char **buckets;
    unsigned capacity;
} HashMap;


CappedQueue* create_capped_queue(unsigned capacity);

HashMap* create_hash_map(unsigned capacity);

unsigned djb_hash(unsigned char *str);

int queue_is_capped(CappedQueue* cq);

void enqueue(CappedQueue* cq, char* key);

#endif // CACHE_H_