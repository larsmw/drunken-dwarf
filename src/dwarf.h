/**/

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

#include "request.h"

using namespace std;

#define CONNMAX 1000
#define BYTES 1024

string ROOT;
int listenfd, clients[CONNMAX];
void error(string);
void startServer(char *);
void respond(int);

