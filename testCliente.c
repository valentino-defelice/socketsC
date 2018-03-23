#include <stdio.h>          
#include <stdlib.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MSGSEND "hola" 
#define RCVBUFSIZE 1024

void DieWithError(char * msg){
	printf("%s\n", msg);
	exit(-1);
}

void main() {

int clientSock, servSock, recvMsgSize, clntLen, echoStringLen;
struct sockaddr_in echoServAddr, echoClntAddr;

char echoString[RCVBUFSIZE];
char echoBuffer[RCVBUFSIZE];

echoServAddr.sin_family = AF_INET;
//echoServAddr.sin_addr.s_addr = inet_pton("localhost");
echoServAddr.sin_port = htons(1111);


	if(inet_pton(AF_INET, "127.0.0.1", &echoServAddr.sin_addr) < 0)
		DieWithError("inet_pton() failed");
	printf("inet_pton\n");

	if((clientSock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		DieWithError("socket() failed");
	printf("clientSock\n");

	if(connect(clientSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("connect() failed");
	printf("connect\n");

	for(;;){
		printf("Yo: ");
		fgets(echoString, sizeof(echoString), stdin);
		
		if(send(clientSock, echoString, RCVBUFSIZE, 0) < 0)
			DieWithError("send() sent a different number of the bytes than expected");
		printf("✓✓\n");

		if((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
			DieWithError("send() sent a different number of the bytes than expected");
		printf("Servidor: %s \n", echoBuffer);
	}
	/*for (;;){
		clntLen = sizeof(echoClntAddr);

		//if(send(clientSock, MSGSEND, sizeof(MSGSEND), 0) != clntLen)
		if(send(clientSock, MSGSEND, sizeof(MSGSEND), 0) != clntLen)
			DieWithError("send() sent a different number of the bytes than expected");

		if((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
			DieWithError("recv() failed");

		while (recvMsgSize < 0) {
			if (send(clientSock, echoBuffer, recvMsgSize, 0) != recvMsgSize)
				DieWithError("send() failed");
			if ((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
				DieWithError("recv() failed");
		}
	} */

close(clientSock);
}
