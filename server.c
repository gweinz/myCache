/* a server in the unix domain.  The pathname of 
   the socket address is passed as an argument */
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void print_arr(char **arr) {
    int i;
    for (i=0;i < (sizeof (arr) /sizeof (arr[0]));i++) {
        printf("arg: %s  \n",arr[i]);
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
            printf("passed action is %s \n", action);
            printf("command is valid\n");
            return 1;
        }
    }
    return -1;
}

void parse_buffer(char *string)
{
    char *token;

    // at most 2 tokens
    char *token_arr[2];
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

    char *action = token_arr[0];
    char *key = token_arr[1];
}

void handle_incoming(int sock) 
{
    int signal;
    char buffer[256];

    bzero(buffer,256);
    signal = read(sock, buffer, 255);

    if (signal < 0) error("ERROR reading from socket");
    printf("Here are the commands...\n");
    parse_buffer(buffer);

    signal = write(sock,"I got your message",18);
    if (signal < 0) error("ERROR writing to socket");
}

int main(int argc, char *argv[])
{
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

    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    while (1) {
        // accept new socket
        child_sockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (child_sockfd < 0) error("ERROR on accept");

        // fork process
        pid = fork();
        if (pid < 0) error("ERROR on fork");

        if (pid == 0) {
            close(sockfd);
            handle_incoming(child_sockfd);
            exit(0);
        } else {
            close(child_sockfd);
        }
    }
    close(sockfd);
    return 0;
}

