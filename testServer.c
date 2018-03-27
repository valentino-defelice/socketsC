#include <stdlib.h>          
#include <stdio.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXPENDING 4
#define RCVBUFSIZE 1024

void DieWithError(char * msg){
	printf("%s\n", msg);
	exit(-1);
}

void main() {

int servSock, clientSock, recvMsgSize, clntLen;
struct sockaddr_in echoServAddr, echoClntAddr;
char echoBuffer[RCVBUFSIZE];
int echoServPort = 1111;

echoServAddr.sin_family = AF_INET;
echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
echoServAddr.sin_port = htons(echoServPort);

	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket() failed pepit");
	printf("serverSock\n");

	if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("bind() failed");
	printf("bind\n");

	if (listen(servSock, MAXPENDING) < 0)
		DieWithError("listen() failed");
	printf("listen\n");

	/*while (recvMsgSize > 0){
		if (send(clientSock, echoBuffer, recvMsgSize, 0) != recvMsgSize)
			DieWithError("send() failed");
	printf("send\n");
		if ((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
			DieWithError("recv() failed");
	printf("recv\n");
	}*/


	for(;;) {
		clntLen = sizeof(echoClntAddr);

		if((clientSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
			DieWithError("accept() failed");
			printf("accept\n");

		if((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
			DieWithError("recv() failed");
			printf("recv\n");
			
		/* while(recvMsgSize > 0) {
			printf("Cliente: %s\n",echoBuffer);
			printf("Yo: \n");
			fgets(echoBuffer, sizeof(echoBuffer), stdin);

			if (send(clientSock, echoBuffer, recvMsgSize, 0) != recvMsgSize)
				DieWithError("send() failed");
				printf("send\n");
			if((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
				DieWithError("recv() failed");
		} */
		for(;;) {
			printf("Cliente: %s\n",echoBuffer);
			printf("Yo: ");
			fgets(echoBuffer, sizeof(echoBuffer), stdin);

			if (send(clientSock, echoBuffer, recvMsgSize, 0) != recvMsgSize)
				DieWithError("send() failed");
				printf("send\n");
			if((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
				DieWithError("recv() failed");
		} 
	close(clientSock);
	}
	return;

}