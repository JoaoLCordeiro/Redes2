#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	main(int argc, char *argv[]){  
	struct sockaddr_in 	endSockServ;
	struct hostent 		*hostP;
	int 	sockID;
	int 	numbytesrecv;
	char 	buff[BUFSIZ+1];
	char 	*host;
	char 	*dados;	
	unsigned int tamEnd;

	if(argc != 4) {
		puts("Uso correto: ./cliente-udp <nome-servidor> <porta> <dados>");
		fprintf(stderr, "Erro:	Número errado de argumentos. Tente:\n./cliente-udp <nome-servidor> <porta> <dados>\n");
		exit(1);
	}

	host	= argv[1];	//pega o nome do host
	dados	= argv[3];	//pega os dados

	if((hostP = gethostbyname(host)) == NULL){
		fprintf(stderr, "Erro:	Não foi possível encontrar o endereço IP do servidor\n");
		exit(1);
	}

	//caso n funcione, h_addr é definido como h_addr_list[0], então pode substituir
	bcopy((char *)hostP->h_addr, (char *)&endSockServ.sin_addr, hostP->h_length);

	endSockServ.sin_family = hostP->h_addrtype;	
	endSockServ.sin_port = htons(atoi(argv[2]));

	if((sockID=socket(hostP->h_addrtype, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "Erro:	Não foi possível abrir o socket\n");
		exit(1);
	}

	if(sendto(sockID, dados, strlen(dados)+1, 0, (struct sockaddr *) &endSockServ, sizeof endSockServ) != strlen(dados)+1){
		fprintf(stderr, "Erro:	Não foi possível enviar os dados\n");
		exit(1);
	}

/* end while }*/

    recvfrom(sockID, buff, BUFSIZ, 0, (struct sockaddr *) &endSockServ, &tamEnd);

    printf("O cliente recebeu a mensagem---->		%s\n", buff);
   
    close(sockID);
    exit(0);

	return 1;
}
