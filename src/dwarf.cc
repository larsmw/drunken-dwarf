#include "dwarf.h"
#include "server.h"

#include <ctime>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;    
    
    //Default Values PATH = ~/ and PORT=10000
    char PORT[6];
    WEB_ROOT = getenv("PWD");
    strcpy(PORT,"10000");

    int slot=0;

    //Parsing the command line arguments
    while ((c = getopt (argc, argv, "p:r:")) != -1)
        switch (c)
        {
            case 'r':
	      //	      WEB_ROOT = (char *)malloc(strlen(optarg));
	      //strcpy(WEB_ROOT,optarg);
		WEB_ROOT = optarg;
                break;
            case 'p':
                strcpy(PORT,optarg);
                break;
            case '?':
                fprintf(stderr,"Wrong arguments given!!!\n");
                exit(1);
            default:
                exit(1);
        }
    
    cout << "Server started at port no. " << PORT << " with root directory as " << WEB_ROOT << endl;
    // Setting all elements to -1: signifies there is no client connected
    for (int i=0; i<CONNMAX; i++)
        clients[i]=-1;


    // Start listening server
    pid_t pid,sid;
    pid = fork();
    if (pid < 0) {
      exit(EXIT_FAILURE);
    }

    // daemon is started...
    if (pid > 0) {
      exit(EXIT_SUCCESS);
    }
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
      /* Log the failure */
      exit(EXIT_FAILURE);
    }

    /* Change the current working directory */
    if ((chdir("/")) < 0) {
      /* Log the failure */
      exit(EXIT_FAILURE);
    }
    
    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    openlog("dwarf", LOG_PID, LOG_USER);
    syslog(LOG_DEBUG, "forked to while");
    while (1)
    {
      int listenfd;
      Server server;

      server.start(PORT, listenfd);
      
      // ACCEPT connections
      //int status = daemon(0, 0);
      addrlen = sizeof(clientaddr);
      cout << inet_ntoa(clientaddr.sin_addr) << endl;
      clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);
      
      if (clients[slot]<0) {
	perror("Error");
      } else {
	server.respond(clients[slot]);
	/*if ( fork()>0 )
	  {
	    openlog("dwarf", LOG_PID, LOG_USER);
	    syslog(LOG_DEBUG, "forked to respond");
	    server.respond(clients[slot]);
	    closelog();
	    exit(0);
	    }*/
      }
      
      while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;
    }
    exit(EXIT_SUCCESS);
}

void error(string e) {
  cerr << e << endl;
}
