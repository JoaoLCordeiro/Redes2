#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TAMFILA      5
#define MAXHOSTNAME 30

int main(int argc, char *argv[]){
	struct	sockaddr_in endSockServ, endSockClie;	//endereço da internet do server e do cliente
	struct	hostent 	*hostP;						//Ponteiro para o host
	unsigned int 		tamEnd;						//armazena o tamanho do endereço
	int 	sockID;									//identifica o socket
    char 	buff [BUFSIZ + 1];						//buffer
	char	localhost [MAXHOSTNAME];				//guarda o nome do host

    if (argc != 2){
    	fprintf(stderr, "Erro:	Número errado de argumentos. Tente:\n./servidor-udp <porta>\n");
    	exit(1);
    }

	gethostname (localhost, MAXHOSTNAME);
	fprintf(stderr, "Servidor ligando...\nServidor ligado :)\nOlá\nMeu nome é %s\nBom te ver, usuário\n", localhost);

	if ((hostP = gethostbyname(localhost)) == NULL){
		fprintf(stderr, "Erro:	Não foi possível encontrar o endereço IP do servidor\n");
		exit (1);
	}	
	
	endSockServ.sin_port = htons(atoi(argv[1]));

	//caso n funcione, h_addr é definido como h_addr_list[0], então pode substituir
	bcopy ((char *) hostP->h_addr, (char *) &endSockServ.sin_addr, hostP->h_length);

	endSockServ.sin_family = hostP->h_addrtype;		


	if ((sockID = socket(hostP->h_addrtype,SOCK_DGRAM,0)) < 0){
        fprintf(stderr, "Erro:	Não foi possível abrir o socket\n");
		exit (1);
	}	

	if (bind(sockID, (struct sockaddr *) &endSockServ,sizeof(endSockServ)) < 0){
		fprintf(stderr, "Erro:	A função bind retornou erro\n");
		exit (1);
	}		
 
    while (1){
       tamEnd = sizeof(endSockClie); 
       fprintf(stdout, "Esperando uma mensagem...\n");
       recvfrom(sockID, buff, BUFSIZ, 0, (struct sockaddr *) &endSockClie, &tamEnd);
       fprintf(stdout, "O servidor recebeu a mensagem---->	%s\n", buff);
       sendto(sockID, buff, BUFSIZ, 0, (struct sockaddr *) &endSockClie, tamEnd);
	}

	return 1;
}