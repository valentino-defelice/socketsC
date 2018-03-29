#include <stdlib.h>          
#include <string.h>          
#include <stdio.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXPENDING 4
#define USER "USER"
#define RCVBUFSIZE 1024
#define HELLOSRV "220 srvFtp version 1.0\r\n"
#define PWRREQ "331 Password required for \r\n"

void DieWithError(char * msg){
	printf("%s\n", msg);
	exit(-1);
}

void main(int argc, char *argv[]) {

FILE *fp;
int servSock, clientSock, recvMsgSize, clntLen;
struct sockaddr_in echoServAddr, echoClntAddr;
char echoBuffer[RCVBUFSIZE];
char echoString[RCVBUFSIZE];
int port = 0;

char usuarioRecibido[20];
char passwordRecibido[20];

char usuario[3][10];
char password[3][10];

echoServAddr.sin_family = AF_INET;
echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(argc != 2)
		DieWithError("Ingrese un Puerto");
	port = atoi(argv[1]);

	if((port < 1) || (port > 65000))
		DieWithError("Puerto Incorrecto, debe ser mayor a 1 y menor a 65000\n");
	echoServAddr.sin_port = htons(port);

	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		DieWithError("socket() failed pepit");
	printf("serverSock\n");

	if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
		DieWithError("bind() failed");
	printf("bind\n");

	if (listen(servSock, MAXPENDING) < 0)
		DieWithError("listen() failed");
	printf("listen\n");

	if((clientSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
		DieWithError("accept() failed");
		printf("accept\n");

	send(clientSock, HELLOSRV, sizeof(HELLOSRV), 0);

	if((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
			DieWithError("send() sent a different number of the bytes than expected");


	//comparando
	char* data = strtok(echoBuffer, " ");
	char datos[20];
	strcpy(datos, data);

	if(strcmp(datos, "USER") != 0){
		printf("Error, codigo recibido: %s\n", datos);
		return -1;
	} 
	else {
		data = strtok(NULL, " ");
		strcpy(usuarioRecibido, data);
		printf("Usuario recibido: %s\n", usuarioRecibido);
	}

	// ABRIENDO Y COPIANDO ARCHIVO
	char buff[100];
	fp = fopen("usuarios.txt", "r");

		int i = 0;
		char* lectura;	//DONDE GUARDAREMOS LOS VALORES OBTENIDOS DEL STRTOK(Usuario y Contraseña)
		fscanf(fp, "%s", buff);

		while(!feof(fp)) {
		
		lectura = strtok(buff, ":"); //LEEMOS Y GUARDAMOS EL PRIMER USUARIO
			strcpy(usuario[i], lectura);

		lectura = strtok(NULL, ":"); //LEEMOS Y GUARDAMOS EL PRIMER USUARIO
			strcpy(password[i], lectura);
		
		fscanf(fp, "%s", buff); //LEEMOS NUEVA LINEA DEL ARCHIVO
		
		i++; 
		}

	//COMPROBANDO USUARIO
		for(i = 0; i < 3; i++) {
	        if(strcmp(usuarioRecibido, usuario[i]) == 0){
	        	snprintf(echoString, sizeof(echoString), "%s%s%s", "331 Password required for: ", usuarioRecibido, " \\r\\n");
	        	send(clientSock, echoString, sizeof(echoString), 0);
	        	break;
        	} 
        	else {
	        	snprintf(echoString, sizeof(echoString), "%s%s%s", "530 Login incorrect for: ", usuarioRecibido, " \\r\\n");
	        	send(clientSock, echoString, sizeof(echoString), 0);
	        }
	    }

		// RECIBIMOS PASSWORD
	    if((recvMsgSize = recv(clientSock, echoBuffer, RCVBUFSIZE, 0)) < 0)
	    	DieWithError("send() sent a different number of the bytes than expected");

	    data = strtok(echoBuffer, " ");
	    strcpy(datos, data);

	    // COMPARANDO CODIGO PASSWORD
	    if(strcmp(datos, "PASS") != 0){
	    	printf("Error, codigo recibido: %s\n", datos);
	    	return -1;
	    } else {
	    	data = strtok(NULL, " ");
	    	strcpy(passwordRecibido, data);
	    				for(i = 0; i < 3; i++) {
	    			        if(strcmp(passwordRecibido, password[i]) == 0){
	    			        	snprintf(echoString, sizeof(echoString), "%s%s%s", "230 User: ", usuarioRecibido[i], "logged in \\r\\n");
	    			        	send(clientSock, echoString, sizeof(echoString), 0);
	    			        	break;
	    		        	} 
	    		        	else {
	    			        	snprintf(echoString, sizeof(echoString), "%s%s%s", "530 Password incorrect for: ", usuarioRecibido[i], " \\r\\n");
	    			        	send(clientSock, echoString, sizeof(echoString), 0);
	    			        }
	    			    }
	    }


	close(clientSock);
	return;
}
