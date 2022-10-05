/* a server in the unix domain.  The pathname of 
   the socket address is passed as an argument */
#include "cache.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void print_queue(CappedQueue* queue) {
    struct Node *tmp;
    tmp = queue->head;
    printf("Begin: \n");
    while (tmp != NULL)
    {
        printf("Node with key of %s has val %d \n", tmp->key, tmp->val);
        tmp = tmp->next;   
    }
}

void unlink_file(const char * filename)
{
    char *mod_file = strdup(filename);
    mod_file[strlen(mod_file)-1] = '\0';
    unlink(mod_file);
}

int check_valid_action(const char *action)
{
    const char *actions[] = {"GET", "PUT", "DEL"};
    int size = sizeof actions / sizeof actions[0]; 
    for (int i = 0; i < size; i++) {
        if (strcmp(action, actions[i]) == 0) {
            return 1;
        }
    }
    return -1;
}

char** parse_buffer(char *string) 
{
    char *token;

    // at most 2 tokens
    char **token_arr = (char **) malloc( 2 * sizeof( char* ) );;
    int count = 0;
    token = strtok(string, " ");
    if (check_valid_action(token) != 1) error("bad request");

    while (token != NULL) //read tokens into the array and increment the counter until all tokens are stored
    {
        if(count > 1) error ("too many args");
        token_arr[count] = token;
        count++;
        token = strtok(NULL, " ");        
    }
    return token_arr;
}


void handle_cache(CappedQueue* queue, HashMap* hash_map, char* command, char* key) 
{
    // TODO: move to Operate function on cache level
    struct Node *res;
    if (strcmp("GET", command) == 0) {
        res = refer(queue, hash_map, key);
    }
    else printf("Deleting key: %s", key);

}

void connection_handler(int sock, CappedQueue* queue, HashMap* hash_map) 
{
    int signal;
    char buffer[256];
    char **args;

    bzero(buffer,256);
    signal = read(sock, buffer, 255);
    if (signal < 0) error("ERROR reading from socket");

    args = parse_buffer(buffer);
    char *c = strdup(args[0]);
    char *k = strdup(args[1]);

    handle_cache(queue, hash_map, c, k);

    signal = write(sock,"I got your message",18);
    if (signal < 0) error("ERROR writing to socket");
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) error("no pathname given");
    int capacity = 10;
    // set up data structures
   
    CappedQueue* queue = create_capped_queue(capacity);

    HashMap* hash_map = create_hash_map(capacity);

    int sockfd, child_sockfd, servlen, pid;
    socklen_t clilen;
    struct sockaddr_un  cli_addr, serv_addr;
    char buf[80];

    // clear out file if exists
    unlink_file(argv[1]);

    if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
        error("ERROR creating socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;

    strcpy(serv_addr.sun_path, argv[1]);
    servlen=strlen(serv_addr.sun_path) + 
                        sizeof(serv_addr.sun_family);

    if(bind(sockfd,(struct sockaddr *)&serv_addr,servlen)<0)
        error("ERROR binding socket"); 

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    do
    { 
        // accept a connection on a socket
        child_sockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        connection_handler(child_sockfd, queue, hash_map);

        print_queue(queue);

        close(child_sockfd);
    } while(1);
    
    close(sockfd);
    return 0;
}

