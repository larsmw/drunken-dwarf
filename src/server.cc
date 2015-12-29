#include "server.h"


//start server
void Server::start(char *port, int &sockfd)
{
    struct addrinfo hints, *res, *p;

    openlog("dwarf", LOG_PID, LOG_USER);
    syslog(LOG_DEBUG, "fetching data from socket");

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
        sockfd = socket (p->ai_family, p->ai_socktype, 0);
        if (sockfd == -1) continue;
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p==NULL)
    {
        perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connections
    if ( listen (sockfd, 1000000) != 0 )
    {
        perror("listen() error");
        exit(1);
    }
}

//client connection
void Server::respond(int client)
{
    char mesg[99999], data_to_send[BYTES], path[99999];
    int rcvd, fd, bytes_read, status;

    std::string msg, response, final;
    list<string> reqline;
    list<string>::iterator str_iter, it;


    // mesg is set to the http request recieved from the client.
    memset( (void*)mesg, (int)'\0', 99999 );
    rcvd=recv(client, mesg, 99999, 0);


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
	send(client, final.c_str(), final.length(), 0);
	status = write(client, response.c_str(), response.length());
    }

    //Closing SOCKET
    shutdown (client, SHUT_RDWR);         
    //All further send and recieve operations are DISABLED...
    close(client);
    client=-1;
}


list<string> Server::tokenize( string const& str,  char const token[])
{
  list<string> results;
  string::size_type j = 0;
  while (j < str.length())
  {
    string::size_type k = str.find(token, j);
    if (k == string::npos)
      k = str.length();

    results.push_back(str.substr(j, k-j));
    j = k + 1;
  }
  return results;
}

string Server::processGetRequest(string strGet)
{
  cout << strGet << endl;
  return "<html><title>Test</title><body><h2>jadaaa...</h2></html>";
}
