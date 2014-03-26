#include<stdio.h>
#include<sys/socket.h>//socket(), connect(), send(), recv()
#include<arpa/inet.h>//for sockaddr_in and inet_addr()
#include<stdlib.h>//for atoi()
#include<string.h>//for memset()
#include<unistd.h>//for close()


#define RCVBUFSIZE 32

void DieWithError(const char *errorMessage);

int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in echoServAddr;
	unsigned short echoServPort;
	char *servIP;
	char *echoString;
	char echoBuffer[RCVBUFSIZE];
	unsigned int echoStringLen;
	int bytesRcvd, totalBytesRcvd;
	if((argc<3) || (argc> 4)){
		fprintf(stderr, "Usage: %s <server ip> <Echo Word> [<Echo Port>]\n", argv[0]);
		exit(1);
	}
	servIP = argv[1]; // server ip address
	echoString = argv[2]; // string to echo
	//get the port
	if(argc == 4){
		echoServPort = atoi(argv[3]);//port
	}else{
		echoServPort = 7;
	}

	//create a stream socket using tcp
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
		DieWithError("socket() failed");
	}

	//construct the server address structure
	memset(&echoServAddr, 0 , sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);
	echoServAddr.sin_port = htons(echoServPort);

	//establish connection the the echo server
	if(connect(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))<0){
	DieWithError("connect() Failed");
	}

	echoStringLen = strlen(echoString);
	if(send(sock, echoString, echoStringLen, 0) != echoStringLen){
		DieWithError("send() problem");
	}

	totalBytesRcvd = 0;
	printf("Received: ");
	while(totalBytesRcvd<echoStringLen){
		if(bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE -1, 0) <= 0){
			DieWithError("recv() failed");
		}
		totalBytesRcvd +=bytesRcvd;
		echoBuffer[bytesRcvd] = '\0';
		printf(echoBuffer);
	}	
	printf("\n");
	close(sock);
	exit(0);

}
void DieWithError(const char *errorMessage){
	perror(errorMessage);
	exit(1);

}



