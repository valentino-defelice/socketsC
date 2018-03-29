#include <stdio.h>          
#include <stdlib.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#define MSGSEND "hola" 
#define ENDCODE "\\r\\n" 
#define USER "USER" 
#define RCVBUFSIZE 1024

void DieWithError(char * msg){
	printf("%s\n", msg);
	exit(-1);
}

void main(int argc, char *argv[]) {

unsigned long ip = 0;
int clientSock, servSock, recvMsgSize, clntLen, echoStringLen;
struct sockaddr_in echoServAddr, echoClntAddr;
int port = 0;


char echoBuffer[RCVBUFSIZE];
char echoString[RCVBUFSIZE];
char username[20];
char password[20];

echoServAddr.sin_family = AF_INET;
//echoServAddr.sin_addr.s_addr = inet_pton("localhost");

	if(argc != 3)
		DieWithError("Ingrese IP y Puerto");
    port = atoi(argv[2]);

	if (0 == inet_pton(AF_INET, argv[1], &ip))
        DieWithError("La IP no es correcta");

    if((atoi(argv[2]) < 1) || (atoi(argv[2]) > 65000))
    	DieWithError("Puerto Incorrecto, debe ser mayor a 1 y menor a 65000\n");
    echoClntAddr.sin_port = htons(port);
	echoServAddr.sin_port = htons(port);

	if(inet_pton(AF_INET, "127.0.0.1", &echoServAddr.sin_addr) < 0)
		DieWithError("inet_pton() failed");

	if((clientSock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		DieWithError("socket() failed");

	if(connect(clientSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("connect() failed");

	if((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
		DieWithError("send() sent a different number of the bytes than expected");
	printf("%s \n", echoBuffer);

	//PEDIMOS USUARIO, FORMATEAMOS MENSAJE Y ENVIAMOS
	printf("username: ");
	gets(username);
	snprintf(echoString, sizeof(echoString), "%s%s%s", "USER ", username, " \\r\\n"); //FORMATEADO DE MSJ

	send(clientSock, echoString, sizeof(echoString), 0);	//ENVIAMOS EL MSJ

	//COMPROBAMOS SI NECESITA PASSWORD
	if((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
		DieWithError("send() sent a different number of the bytes than expected");

	//comparando
	char* data = strtok(echoBuffer, " ");
	char datos[20];
	strcpy(datos, data);

	if(strcmp(datos, "331") == 0){
		printf("password: ");
		gets(password);
		snprintf(echoString, sizeof(echoString), "%s%s%s", "PASS ", password, " \\r\\n"); //FORMATEADO DE MSJ

		send(clientSock, echoString, sizeof(echoString), 0);	//ENVIAMOS EL MSJ
		printf("%s\n", echoString);
	}


close(clientSock);
}
