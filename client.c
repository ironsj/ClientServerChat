#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#define MAX 1000

void error(char *msg){
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[]){
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[MAX];
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	
	// assign port number
	portno = atoi(argv[2]);
	
	// create and verifies socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){ 
		error("ERROR opening socket");
	}
	
	// get server info
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	// assign IP address
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	
	// connect client socket to server
	if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){ 
		error("ERROR connecting");
	}
	else{
		printf("Connected to server...\n");
	}
	
	// loop for client-server chatting
	while(1){
		bzero(buffer, MAX);
		printf("Enter message to be sent to server: ");
		
		// put client message into buffer
		fgets(buffer, sizeof(buffer), stdin);
		
		// send message to the server
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0){ 
	 		error("ERROR writing to socket");
	 	}
	 	
	 	// break the loop if client ends the connection
		if (strcmp("end\n", buffer) == 0) {
			printf("Exiting chat...\n");
	 		break;
		}
		
		bzero(buffer,256);
		
		// read message from the server and copy into biffer
		n = read(sockfd,buffer, sizeof(buffer));
		if (n < 0){ 
	 		error("ERROR reading from socket");
	 	}
	 	
	 	// break the loop if client ends the connection
		if (strcmp("end\n", buffer) == 0) {
			printf("Server has disconnected...\n");
	 		break;
		}
	 	
	 	// print message from the server
		printf("Server: %s\n",buffer);

	}
	
	// close socket after chat ends
	close(sockfd);
	return 0;
}

