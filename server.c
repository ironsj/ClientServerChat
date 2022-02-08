/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#define MAX 1000

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;
	
	char buffer[MAX];
	int n;
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	
	// creates and verifies socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){ 
		error("ERROR opening socket");
	}
	
	// assigns IP address and port number
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	// bind socket to IP
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){ 
		error("ERROR on binding");
	}
	
	// server listens for incoming connection requests
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	
	// accept data packet from client
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0){ 
		error("ERROR on accept");
	}
	else{
		printf("Accepting client...\n");
	}

	//loop for client-server chatting
	while(1){
		bzero(buffer, MAX);
		
		// read message from the client and copy into buffer
		n = read(newsockfd, buffer, sizeof(buffer));
		if (n < 0){ 
			error("ERROR reading from socket");
		}
		
		// break from loop if client ends connection
		if (strcmp("end\n", buffer) == 0) {
			printf("Client has disconnected...\n");
			break;
		}
		
		// print the message from the client
		printf("Client: %s\n",buffer);

		bzero(buffer, MAX);
		printf("Enter message to be sent to client: ");
		
		// put the server message into the buffer
		fgets(buffer, sizeof(buffer), stdin);
		
		//send message to the client
		n = write(newsockfd,buffer, sizeof(buffer));
		if (n < 0){
			error("ERROR writing to socket");
		}
		
		// break from loop if server ends the connection
		if (strcmp("end\n", buffer) == 0) {
			printf("Exiting chat...\n");
			break;
		}
	}
	
	//close socket after chat ends
	close(sockfd);
	return 0; 
}

