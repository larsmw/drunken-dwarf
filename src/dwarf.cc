#include "dwarf.h"

#include <ctime>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;    
    
    //Default Values PATH = ~/ and PORT=10000
    char PORT[6];
    ROOT = getenv("PWD");
    strcpy(PORT,"10000");

    int slot=0;

    //Parsing the command line arguments
    while ((c = getopt (argc, argv, "p:r:")) != -1)
        switch (c)
        {
            case 'r':
	      //	      ROOT = (char *)malloc(strlen(optarg));
	      //strcpy(ROOT,optarg);
		ROOT = optarg;
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
    cout << "Server started at port no. " << PORT << " with root directory as " << ROOT << endl;
    // Setting all elements to -1: signifies there is no client connected
    int i;
    for (i=0; i<CONNMAX; i++)
        clients[i]=-1;
    startServer(PORT);

    // ACCEPT connections
    while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);

        if (clients[slot]<0)
            error ("accept() error");
        else
        {
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

//start server
void startServer(char *port)
{
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port, &hints, &res) != 0)
    {
        perror ("getaddrinfo() error");
        exit(1);
    }
    // socket and bind
    for (p = res; p!=NULL; p=p->ai_next)
    {
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p==NULL)
    {
        perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if ( listen (listenfd, 1000000) != 0 )
    {
        perror("listen() error");
        exit(1);
    }
}

//client connection
void respond(int n)
{
    char mesg[99999], data_to_send[BYTES], path[99999];
    int rcvd, fd, bytes_read;

    string msg, response, final;
    list<string> reqline;
    list<string>::iterator str_iter;


    // mesg is set to the http request recieved from the client.
    memset( (void*)mesg, (int)'\0', 99999 );
    rcvd=recv(clients[n], mesg, 99999, 0);


    if (rcvd<0)    // receive error
      fprintf(stderr,("recv() error\n"));
    else if (rcvd==0)    // receive socket closed
      fprintf(stderr,"Client disconnected upexpectedly.\n");
    else    // message received
      {
	time_t reqTime = time(NULL);
	reqline = tokenize(mesg, " \t\n");
	for(str_iter = reqline.begin(); str_iter != reqline.end(); ++str_iter) {
	  // Each line in the http-header
	  cout << ctime(&reqTime) << *str_iter << endl;
	  if(*str_iter.front() == "GET ")
	    processGetRequest(*str_iter.front());
	  response = "<h1>HTTP 1.0 requested</h1>";
	  //else response = *str_iter;
	}
	final = "HTTP/1.0 200 OK \n\n";
	cout << final << endl;
	//	send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
	send(clients[n], final.c_str(), final.length(), 0);
	write(clients[n] , response.c_str(), response.length());
	/*        if ( strncmp(reqline[0], "GET\0", 4)==0 )
        {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");
            if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
            {
                write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
            }
            else
            {
	                      if ( strncmp(reqline[1], "/\0", 2)==0 )
		  reqline += "/index.html";
		//                    reqline[1] = "/index.html"; 
		// Because if no file is specified, index.html will be opened 
		// by default (like it happens in APACHE...

                strcpy(path, ROOT);
                strcpy(&path[strlen(ROOT)], reqline[1]);
                printf("file: %s\n", path);

                if ( (fd=open(path, O_RDONLY))!=-1 )    //FILE FOUND
                {
                    send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
                        write (clients[n], data_to_send, bytes_read);
                }
                else    write (clients[n], "HTTP/1.0 404 Not Found\n", 23); //FILE NOT FOUND
            }
	    }*/
    }

    //Closing SOCKET
    shutdown (clients[n], SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(clients[n]);
    clients[n]=-1;
}

void error(string e) {
  cerr << e;
}
