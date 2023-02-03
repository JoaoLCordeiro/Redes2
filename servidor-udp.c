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

	fprintf(stderr, "Servidor ligado :)\nOlá\nMeu nome é %s\nBom te ver, usuário\n\n", localhost);	

	

	while (1){
		//while maior /\, representa o servidor rodando

		//primeira mensagem informa quantos números chegarão
		recvfrom(sockID, buff, BUFSIZ, 0, (struct sockaddr *) &endSockClie, &tamEnd);
		int		n_msg	= atoi(buff);

		int		num_recebido;
		int		num_anterior	= -1;
		int		maior_recebido	= 0;
		int*	chegou			= (int *) calloc (n_msg,sizeof(int));
		int*	desord			= (int *) malloc (n_msg*sizeof(int));	//resolver no cpp? vetor de tamanho variavel
		int		indic_desord	= 0;

		//while menor \/, representa o recebimento das mensagens
    	while (1){
			//versão original do site do elias \/
    		/*tamEnd = sizeof(endSockClie); 
    		fprintf(stdout, "Esperando uma mensagem...\n");
    		recvfrom(sockID, buff, BUFSIZ, 0, (struct sockaddr *) &endSockClie, &tamEnd);
    		fprintf(stdout, "O servidor recebeu a mensagem---->	%s\n", buff);
    		sendto(sockID, buff, BUFSIZ, 0, (struct sockaddr *) &endSockClie, tamEnd);*/

			//implementar um timeout que sai desse while caso, depois da primeira mensagem, ele pare de receber msg
			//isso quer dizer q as mensagens acabaram
			recvfrom(sockID, buff, BUFSIZ, 0, (struct sockaddr *) &endSockClie, &tamEnd);
			num_recebido = atoi(buff);

			if (num_recebido != num_anterior + 1){
				desord[indic_desord] = num_recebido;
				indic_desord++;
			}

			chegou[num_recebido] = 1;

			num_anterior = num_recebido;
		}

		//a partir daqui, no vetor "chegou", os indices com 0 não chegaram
		//e o vetor desord guarda quem não chegou depois do numero anterior

		for (int i = 0 ; i < n_msg ; i++)
			//caso a mensagem n chegou
			if (! chegou[i])
				//talvez mandar isso para um arquivo ao invés da stdout
				fprintf(stdout, "A mensagem de número %d não chegou\n", i);	

		for (int i = 0 ; i < indic_desord ; i++)
			//talvez mandar isso para um arquivo ao invés da stdout
			fprintf(stdout, "A mensagem de número %d chegou fora de ordem\n", i);	
	}


	return 1;
}