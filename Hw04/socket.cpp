#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <iostream>

#include "socket.h"

using namespace std;

int serverTCP(int listenPORT){
	int sockfd , newsockfd;
	struct sockaddr_in serv_addr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)  // 建立sockfd .
        perror("server erro!");

    bzero((char*)&serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY : 可接受任何Client .
    serv_addr.sin_port = htons(listenPORT);

    // bind 前須加 '::' , 否則會和std的bind搞混 .
    if(::bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <0) // bind sock & port .
        perror("bind error");

    listen(sockfd, 1); // listen client .

    struct sockaddr_in cli_addr;
    socklen_t clilen;
	
	cout << "Waiting for client connection ..." << endl;
	newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen); // accept , 並建立新fd來服務該client .

	return newsockfd;
}

int clientTCP(const char *serverName, int serverPORT){
	
	struct hostent *he; 

	if((he=gethostbyname(serverName)) == NULL){
	 	fprintf(stderr,"Usage : ./othello -c <IP-of-Player-1>:<Port-of-Player-1>");
	 	exit(1);
	}

	int    client_fd;
	struct sockaddr_in client_sin;

	client_fd = socket(AF_INET,SOCK_STREAM,0);

	bzero(&client_sin,sizeof(client_sin));
	client_sin.sin_family = AF_INET;
	client_sin.sin_addr = *((struct in_addr *)he->h_addr); 
	client_sin.sin_port = htons(serverPORT);

	if(connect(client_fd,(struct sockaddr *)&client_sin,sizeof(client_sin)) == -1){
  		perror("Client connet to server error !");
  		exit(1);
 	}

 	return client_fd;
}





