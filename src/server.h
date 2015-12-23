#ifndef __SERVER_H__
#define __SERVER_H__

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>

#include <iostream>
#include <string>


class Server {
  int listenfd;
public:
  void start(char *port, int &sockfd);
};

#endif
