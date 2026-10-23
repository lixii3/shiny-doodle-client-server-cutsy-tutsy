#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "ft_split.c"

#define SERVERPORT 12123
#define SERVERADDRESS INADDR_ANY

#define MAX 32

int main(int argc, char * argv[], char **envp) {
	int serverSocket, connectSocket, returnCode;
	socklen_t clientAddressLen;
	struct sockaddr_in serverAddress, clientAddress;
	char buffer[MAX];
	char *clientIP;
	char	**args;

	char *benvenuto="Benvenuto!";

	/* apertura socket del server */
	if (((serverSocket = socket(AF_INET,SOCK_STREAM,0))) == -1) {
	perror("Error in server socket()");
	exit(-1);
	}

	/* preparazione dell'indirizzo locale del server */
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVERPORT);
	serverAddress.sin_addr.s_addr = SERVERADDRESS;

	/* bind del socket */
	if ((returnCode = bind (serverSocket, (struct sockaddr*)	&serverAddress, sizeof(serverAddress))) == -1) {
	perror("Error in server socket bind()");
	exit(-1);
	}

	/* listen del socket */
	if ((returnCode = listen(serverSocket, 5)) == -1) {
	perror("Error in server socket listen()");
	exit(-1);
	}

	printf("Server ready (CTRL-C per terminare)\n");

	clientAddressLen = sizeof(clientAddress);
	/* ciclo infinito in attesa di connessione */
	while (1) {

	/* accetta le connessioni */
	if ((connectSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen)) == -1) {
		perror("Error in accept()");
		close(serverSocket);
		exit(-1);
	}
	if(fork()==0){
		/* memorizzo l'indirizzo del client in formato dots */
		clientIP = inet_ntoa(clientAddress.sin_addr);

		printf("\nClient @ %s connects on socket %d\n", clientIP, connectSocket);

		/* riceve i messaggi e li stampa in output */
		while ((returnCode = read(connectSocket, buffer, MAX-1)) > 0) {
			buffer[returnCode] = '\0'; // buffer dev'essere una stringa e va terminata con '\0'
			printf(">> %s: %s\n", clientIP, buffer);
			write(connectSocket, benvenuto,strlen (benvenuto) + 1);
			
			// eheheheheheeheheh this can be hurtful
			args = ft_split(buffer, ' ');
			if (fork() == 0)
			{
				if(execve(args[0], args, envp) == -1)
					perror("execve");
			}
			free(args);
		}
	}else
	/* chiudo il socket */
	close(connectSocket);
	}
	return(0);
}

