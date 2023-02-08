#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>		//implementa o timeout

#define TIMEOUT		1000
#define TAMNOMEARQ	30

#define TAMFILA      5
#define MAXHOSTNAME 30

int main(int argc, char *argv[]){
	struct	sockaddr_in endSockServ, endSockClie;	//endereço da internet do server e do cliente
	struct	hostent 	*hostP;						//ponteiro para o host
	unsigned int 		tamEnd;						//armazena o tamanho do endereço
	int 	sockID;									//identifica o socket
    char 	buff [BUFSIZ + 1];						//buffer
	char	localhost [MAXHOSTNAME];				//guarda o nome do host

	//verifica o numero de argumentos
    if (argc != 2){
    	fprintf(stderr, "Erro:	Número errado de argumentos. Tente:\n./servidor-udp <porta>\n");
    	exit(1);
    }

	//consegue a string que representa o server
	gethostname (localhost, MAXHOSTNAME);

	if ((hostP = gethostbyname(localhost)) == NULL){
		fprintf(stderr, "Erro:	Não foi possível encontrar o endereço IP do servidor\n");
		exit (1);
	}	
	
	//pega a porta dada pelo usuário do servidor
	endSockServ.sin_port = htons(atoi(argv[1]));

	bcopy ((char *) hostP->h_addr, (char *) &endSockServ.sin_addr, hostP->h_length);

	endSockServ.sin_family = hostP->h_addrtype;		

	//abre o socket
	if ((sockID = socket(hostP->h_addrtype,SOCK_DGRAM,0)) < 0){
        fprintf(stderr, "Erro:	Não foi possível abrir o socket\n");
		exit (1);
	}

	if (bind(sockID, (struct sockaddr *) &endSockServ,sizeof(endSockServ)) < 0){
		fprintf(stderr, "Erro:	A função bind retornou erro\n");
		exit (1);
	}

	//notifica o sucesso da inicialização do servidor
	fprintf(stdout, "Servidor ligado :)\nOlá\nMeu nome é %s\nBom te ver, usuário\n\nAguardando mensagens...\n\n", localhost);

	//usado para verificar timeout
	struct pollfd pfd;
	pfd.fd		= sockID;
	pfd.events	= POLLIN;

	while (1){
		//while maior /\, representa o servidor rodando


		//primeira mensagem informa quantos números chegarão
		recvfrom(sockID, buff, BUFSIZ, 0, (struct sockaddr *) &endSockClie, &tamEnd);

		//criando arquivos de log
		char* nom_arq_nchegou	= (char *) malloc (TAMNOMEARQ * sizeof(char));
		char* nom_arq_desorde	= (char *) malloc (TAMNOMEARQ * sizeof(char));

		//cria as strings que darao nome aos arquivos de logs
		strcpy (nom_arq_nchegou, "c-nchegou-");
		strcpy (nom_arq_desorde, "c-desorde-");
		strcat (nom_arq_nchegou, buff);
		strcat (nom_arq_desorde, buff);
		strcat (nom_arq_nchegou, ".txt");
		strcat (nom_arq_desorde, ".txt");

		//cria os arquivos
		FILE* f_nchegou = fopen (nom_arq_nchegou,"w+");
		FILE* f_desorde = fopen (nom_arq_desorde,"w+");

		//pega o número de mensagens
		long int n_msg	= atoi(buff);

		//detecta erro na abertura de arquivos
		if ((! f_nchegou) || (! f_desorde)){
			fprintf(stderr, "A tentativa de criar os arquivos de log para %ld mensagens falhou\n", n_msg);
			exit(1);
		}

		//cria as variáveis usadas no processo de recebimento de mensagens
		long int	num_recebido;
		long int	num_anterior	= -1;
		char*		chegou			= (char *) calloc (n_msg,sizeof(char));		//guarda se a mensagem do indice chegou
		int*		desord			= (int *) malloc (n_msg*sizeof(int));		//guarda as mensagens que chegaram desordenadas
		long int	indic_desord	= 0;

		//while menor \/, representa o recebimento das mensagens
    	while (1){
			//timeout
			int retorno_poll	= poll(&pfd, 1, TIMEOUT);
			if (retorno_poll == 0)	//detecta timeout, as mensagens acabaram
				break;

			recvfrom(sockID, buff, BUFSIZ, 0, (struct sockaddr *) &endSockClie, &tamEnd);
			num_recebido = atoi(buff);

			//detecta se a mensagem está fora de ordem
			if (num_recebido < num_anterior){
				desord[indic_desord] = num_recebido;
				indic_desord++;
			}

			//registra que a mensagem chegou
			chegou[num_recebido] = 1;

			num_anterior = num_recebido;
		}
		fprintf(stdout, "O recebimento de %ld mensagens acabou, esperando mais...\n", n_msg);

		//a partir daqui, no vetor "chegou", os indices com 0 não chegaram
		//e o vetor desord guarda quem não chegou depois do numero anterior

		//escreveremos nos arquivos de log
		for (long int i = 0 ; i < n_msg ; i++)
			//caso a mensagem n chegou
			if (! chegou[i])
				fprintf(f_nchegou, "A mensagem de número %ld não chegou\n", i);	

		for (long int i = 0 ; i < indic_desord ; i++)
			fprintf(f_desorde, "A mensagem de número %ld chegou fora de ordem\n", i);	

		//limpa os vetores e fecha os arquivos
		free(chegou);
		free(desord);
		fclose (f_nchegou);
		fclose (f_desorde);
	}


	return 1;
}