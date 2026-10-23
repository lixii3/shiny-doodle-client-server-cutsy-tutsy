#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#ifndef SERVERPORT
#define SERVERPORT 12123
#endif

#define SERVERADDRESS "127.0.0.1"

int main(int argc, char *argv[]) {
	int clientSocket;
	struct sockaddr_in serverAddress;
	char *msg;
	const char *serverIP;
	const char *message;
	size_t msglen;
	int returnCode;

	char buffer[20];

	if (argc == 2) {
		serverIP = SERVERADDRESS;
		message = argv[1];
	} else if (argc == 3) {
		serverIP = argv[1];
		message = argv[2];
	} else {
		printf("Usage: %s [server-ip] \"message to send\"\n", argv[0]);
		exit(-1);
	}

	/* preparo la variabile msg in base alla dimensione del messaggio inserito dall'utente */
	msg = malloc(strlen(message) + 1);
	if (msg == NULL) {
		perror("Error allocating message");
		exit(-1);
	}

	/* apertura socket del client */
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Error in socket()");
		exit(-1);
	}

	/* preparazione dell'indirizzo del server con cui connettere il socket */
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVERPORT);
	if (inet_pton(AF_INET, serverIP, &serverAddress.sin_addr) != 1) {
		fprintf(stderr, "Invalid server IPv4 address: %s\n", serverIP);
		close(clientSocket);
		free(msg);
		exit(-1);
	}

	/* connessione del socket al server */
	if ((connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress))) == -1) {
		perror("Error in connect()");
		exit(-1);
	}

	printf("Client connected to %s:%d\n", serverIP, SERVERPORT);

	/* invio messaggio al server */
	strcpy(msg, message);
	msglen = strlen(msg)+1;
	returnCode = write(clientSocket, msg, msglen);

	printf("Sent %d (%u request) bytes on socket %d\n", returnCode, (unsigned) msglen, clientSocket);

	read(clientSocket,buffer,20);
	printf("From Server: %s \n",buffer);
	close(clientSocket);
	return(0);
}
