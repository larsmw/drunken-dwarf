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
    
    //    printf("Server started at port no. %s%s%s with root directory as %s%s%s\n","\033[92m",PORT,"\033[0m","\033[92m",ROOT,"\033[0m");
    cout << "Server started at port no. " << PORT << " with root directory as " << WEB_ROOT << endl;
    // Setting all elements to -1: signifies there is no client connected
    int i;
    for (i=0; i<CONNMAX; i++)
        clients[i]=-1;

    int listenfd;
    Server server;

    server.start(PORT, listenfd);

    // ACCEPT connections
    //int status = daemon(0, 0);
    while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[slot]<0) {
	  perror("Error");
	} else {
	  if ( fork()==0 )
            {
	      respond(slot);
	      exit(0);
            }
        }

        while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;
    }

    return 0;
}

//client connection
void respond(int n)
{
    char mesg[99999], data_to_send[BYTES], path[99999];
    int rcvd, fd, bytes_read, status;

    std::string msg, response, final;
    list<string> reqline;
    list<string>::iterator str_iter, it;


    // mesg is set to the http request recieved from the client.
    memset( (void*)mesg, (int)'\0', 99999 );
    rcvd=recv(clients[n], mesg, 99999, 0);


    if (rcvd<0)    // receive error
      fprintf(stderr,("recv() error\n"));
    else if (rcvd==0)    // receive socket closed
      fprintf(stderr,"Client closed connection.\n");
    else    // message received
      {
	time_t reqTime = time(NULL);
	//cout << "req : " << mesg << endl << "req end" << endl;
	reqline = tokenize(mesg, "\n");
	for(str_iter = reqline.begin(); str_iter != reqline.end(); ++str_iter) {
	  // Each line in the http-header
	  std::string s;
	  s = str_iter->c_str();
	  //cout << ctime(&reqTime) << s << endl;
	  if(s.find("GET ")) {
	    response = processGetRequest(*str_iter);
	    //cout << *str_iter << endl;
	  } else {
	    response = *str_iter;
	  }
	}
	final = "HTTP/1.0 200 OK \n\n";
	//cout << final << endl;
	//	send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
	send(clients[n], final.c_str(), final.length(), 0);
	status = write(clients[n], response.c_str(), response.length());
    }

    //Closing SOCKET
    shutdown (clients[n], SHUT_RDWR);         
    //All further send and recieve operations are DISABLED...
    close(clients[n]);
    clients[n]=-1;
}

void error(string e) {
  cerr << e << endl;
}
