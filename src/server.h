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
#include<syslog.h>

#include <iostream>
#include <string>
#include <list>

using namespace std;

#define BYTES 1024

class Server {
  int listenfd;
  list<string> tokenize(  string const& str,  char const token[]);
public:
  void start(char *port, int &sockfd);
  void respond(int client);
  string processGetRequest(string strGet);
};

#endif
