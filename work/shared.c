#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "cache.h"
#include <string.h>

void print_queue(CappedQueue* queue) {
    struct Node *tmp;
    tmp = queue->head;
    while (tmp != NULL)
    {
        printf("Node = %s\n", tmp->key);
        tmp = tmp->next;   
    }
}

 while (1) {
        // accept new socket
        child_sockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (child_sockfd < 0) error("ERROR on accept");

        // fork process
        pid = fork();
        if (pid < 0) error("ERROR on fork");

        if (pid == 0) {
            close(sockfd);
            process(child_sockfd, queue, hash_map);
            print_queue(queue);
            exit(0);
        } else {
            wait(NULL);
            print_queue(queue);
            close(child_sockfd);
        }
    }

int main()
{
    // CappedQueue* queue = (CappedQueue*) mmap(NULL, sizeof (CappedQueue) , PROT_READ | PROT_WRITE,
    //                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    CappedQueue *var;

    char* alph[10] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
    var = mmap(NULL, sizeof *var, PROT_READ | PROT_WRITE, 
                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *var = *create_capped_queue(10);
    for(int i=0;i<5;i++) // loop will run n times (n=5)
    {
        if(fork() == 0)
        {
            enqueue(*var, alph[i]);
            printf("head of queue %s \n", var->head->key);
            printf("next of head of queue %s \n", var->head->next->key);
            printf("tail of queue %s \n", var->tail->key);

            // printf("[son] pid %d from [parent] pid %d\n", getpid(),getppid());
            exit(0);
        }
    }
    for(int i=0;i<5;i++) // loop will run n times (n=5)
    wait(NULL);

      
}

// int main()
// {
//     static int *glob_var;
//     glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, 
//                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);

//     *glob_var = 1;
//     int pid;
//     pid = fork();   
//     if (fork() == 0) {
//         *glob_var = 5;
//         exit(EXIT_SUCCESS);
//     } else {
//         wait(NULL);
//         printf("%d\n", *glob_var);
//         munmap(glob_var, sizeof *glob_var);
//     }
//     return 0;
// }