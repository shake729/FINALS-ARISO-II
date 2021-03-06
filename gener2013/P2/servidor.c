#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SIZE 80

int main (int argc, char *argv[]){
	int i, num_chars;
	int sockorig, socknew, lenclient, len;
	int  PORT=12;
	char mensaje[SIZE];
	struct sockaddr_in server, client;

	if ( (sockorig = socket (AF_INET, SOCK_STREAM,0)) < 0){
		fprintf(stderr, "ERROR socket\n");
		exit(1);
	}

	memset (&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	inet_aton("5.6.7.8", &(server.sin_addr));
	server.sin_port = htons(PORT);

	if ( bind(sockorig, (struct sockaddr*) &server, sizeof(server)) < 0){
		fprintf(stderr,"ERROR bind\n");
		exit(1);
	}
	if ( listen(sockorig, 5) < 0){
		fprintf(stderr,"ERROR listen\n");
		exit(1);
	}

	for (;;){
		lenclient = sizeof(client);
		if ( (socknew = accept (sockorig, (struct sockaddr*) &client, &lenclient)) < 0){
			fprintf(stderr,"ERROR accept\n");
			exit(1);
		}
		switch (fork()){
			case -1: fprintf(stderr,"ERROR fork\n"); exit(1);
			case 0:
					close(sockorig);
					while ( (len = read (socknew, mensaje, SIZE)) > 1){
						num_chars = htonl(len);
						if (write (socknew, &num_chars, sizeof(int)) < 0){
							fprintf(stderr,"ERROR write\n");
							exit(1);
						}
						memset(mensaje,0,SIZE);
					}
					num_chars = 0;
					write (socknew, &num_chars, sizeof(int));
					close(socknew);
					exit(0);
			default: close (socknew);
		}
	}
}






