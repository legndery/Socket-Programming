#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>


#define MAXPENDING 5

void DieWithError(const char *errorMessage){
	perror(errorMessage);
}

void HandleTCPClient(int clntSocket);

int main(int argc, char *argv[]){
	int servSock;
	int clntSock;
	struct sockaddr_in echoservAddr;
	struct sockaddr_in echoClntAddr;
	unsigned short echoServPort;
	unsigned int clntLen;

	if(argc != 2){
		fprintf(stderrm, "Usage: %s <server Port>\n", argv[0]);
		exit(1);
	}

	echoServPort = atoi(argv[1]);

	if((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
		DieWithError("socket() failed");
	}

	//local address structure
	memset(&echoservAddr, 0, sizeof(echoservAddr));
	echoServAddr.sin_family = AF_INET;
	echoservAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	echoservAddr.sin_port = htons(echoServPort);

	if(bind(servSock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))<0){
		DieWithError("bind() failed");
	}

	if(listen(servSock,MAXPENDING)<0)
		DieWithError("listen() failed");

	for(;;){
		clntLen = sizeof(echoClntaddr);

		if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,&clntLen))<0)
		DieWithError("accept() failed");
		printf("handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

		HandleTCPClient(clntSock);
	
	}

	exit(0);
}
