/*dwarf.h*/
#ifndef __DWARF_W__
#define __DWARF_W__

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

#include "request.h"

using namespace std;

#define CONNMAX 1000

string WEB_ROOT;
int clients[CONNMAX];

void error(string);
void respond(int);

#endif
