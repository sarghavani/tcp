// Copyright 2023 Sajad Arghavani
/*
 * This is a simple program to connect the client to the server in the socket
 *  programing with epoll usage edge-triggerd and non-block in C languge.
 *
 *  Author: Sajad Arghavani<sajjadarghavani@gmail.com>
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>


#define BUFFERSIZE 100
#define IPSERVER "127.0.0.1"
#define PORT 8888
#define FAILED -1


// Variable to store the result of operations on the socket
int responses;


int
createsocket() {
    responses = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if (responses == FAILED) {
        perror("socket create failed");
    }

    return responses;
}


int
configureaddr(struct sockaddr_in *serveraddr) {
    serveraddr->sin_family = AF_INET;
    serveraddr->sin_port = htons(PORT);
    responses = inet_pton(AF_INET, IPSERVER, &(serveraddr->sin_addr));

    if (responses == FAILED) {
        perror("inet_pton failed");
    }

    return responses;
}


int
connecttoserver(int clientfd, struct sockaddr *serveraddr) {
    responses = connect(clientfd, serveraddr, sizeof(*serveraddr));

    if (responses == FAILED) {
        if (errno != EINPROGRESS) {
            perror("connect to server failed");
        }
    }

    return responses;
}


int
createepoll() {
    responses = epoll_create1(0);

    if (responses == FAILED) {
        perror("epoll_create1 failed");
    }

    return responses;
}


int
addsockettoepoll(int clientfd, int epollfd) {
    struct epoll_event event;

    event.events = EPOLLIN | EPOLLET;
    event.data.fd = clientfd;
    responses = epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &event);

    if (responses == FAILED) {
        perror("epoll_ctl failed");
    }

    return responses;
}


int
sendmessage(int clientfd, char *message) {
    responses = send(clientfd, message, strlen(message), 0);

    if (responses == FAILED) {
        perror("send message for client failed");
    }

    return responses;
}


void
handlerecv(struct epoll_event *events) {
    char buffer[BUFFERSIZE];

    if (events[0].events & EPOLLIN) {
        responses = recv(events[0].data.fd, buffer, BUFFERSIZE, 0);

        if (responses == FAILED) {
            perror("recv failed");
        }
        else if (responses == 0) {
            printf("Server disconnected.\n");
            return;
        }
    }
}


int
epollwait(int epollfd) {
    int maxevent;
    int timeout;
    struct epoll_event event;

    maxevent = 1;
    timeout = -1;
    responses = epoll_wait(epollfd, &event, maxevent, timeout);

    if (responses == FAILED) {
        perror("epoll_wait failed");
    }

    handlerecv(&event);

    return responses;
}


int
main() {
    int clientfd;
    int epollfd;
    char message[BUFFERSIZE];
    struct sockaddr_in serveraddr;

    clientfd = createsocket();
    configureaddr(&serveraddr);
    connecttoserver(clientfd, (struct sockaddr *)&serveraddr);
    epollfd = createepoll();
    addsockettoepoll(clientfd, epollfd);

    printf("Enter a message to send to the server\n");
    fgets(message, BUFFERSIZE, stdin);

    sendmessage(clientfd, message);
    epollwait(epollfd);
    close(clientfd);
    close(epollfd);

    return 0;
}
