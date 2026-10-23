#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERVERPORT 12345
#define SERVERADDRESS "127.0.0.1"

int main(int argc, char *argv[]) {
	int clientSocket;
	struct sockaddr_in serverAddress;
	char *msg;
	size_t msglen;
	int returnCode;

	char buffer[20];

	if (argc != 2) {
		printf("Usage: %s \"message to send\"\n", argv[0]);
		exit(-1);
	}

	/* preparo la variabile msg in base alla dimensione del messaggio inserito dall'utente */
	msg = malloc(strlen(argv[1])*sizeof(char));

	/* apertura socket del client */
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error in socket()");
		exit(-1);
	}

	/* preparazione dell'indirizzo del server con cui connettere il socket */
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVERPORT);
	serverAddress.sin_addr.s_addr = inet_addr(SERVERADDRESS);

	/* connessione del socket al server */
	if ((connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress))) == -1) {
		perror("Error in connect()");
		exit(-1);
	}

	printf("Client connected to %s\n", SERVERADDRESS);

	/* invio messaggio al server */
	strcpy(msg, argv[1]); 
	msglen = strlen(msg)+1;
	returnCode = write(clientSocket, msg, msglen);

	printf("Sent %d (%u request) bytes on socket %d\n", returnCode, (unsigned) msglen, clientSocket);

	read(clientSocket,buffer,20);
	printf("From Server: %s \n",buffer);
	close(clientSocket);
	return(0);
}

