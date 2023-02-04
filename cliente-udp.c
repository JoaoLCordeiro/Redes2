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
	long int		num_msg;	
	unsigned int	tamEnd;

	if(argc != 4) {
		puts("Uso correto: ./cliente-udp <nome-servidor> <porta> <num_msg>");
		fprintf(stderr, "Erro:	Número errado de argumentos. Tente:\n./cliente-udp <nome-servidor> <porta> <dados>\n");
		exit(1);
	}

	host	= argv[1];			//pega o nome do host
	num_msg	= atoi(argv[3]);	//pega o num de msgs

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

	sprintf(buff, "%d", num_msg);

	if(sendto(sockID, buff, strlen(buff)+1, 0, (struct sockaddr *) &endSockServ, sizeof endSockServ) != strlen(buff)+1){
		fprintf(stderr, "Erro:	Não foi possível enviar a mensagem inicial\n");
		exit(1);
	}

	for (long int i_msg = 0 ; i_msg < num_msg ; i_msg++){
		sprintf(buff, "%d", i_msg);
		if(sendto(sockID, buff, strlen(buff)+1, 0, (struct sockaddr *) &endSockServ, sizeof endSockServ) != strlen(buff)+1){
			fprintf(stderr, "Erro:	Não foi possível enviar a mensagem %d\n", i_msg);
			exit(1);
		}
	}

    close(sockID);
    exit(0);

	return 1;
}
