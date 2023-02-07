import sys
import socket
import pickle

BUFFERSIZE = 1024

#main do servidor
if __name__ == "__main__":
	if len(sys.argv) != 2:
		print(f"Erro:	Número errado de argumentos. Tente:\npython servidor-udp.py <porta>")
		exit(1)

	#pega a porta dos parametros da chamada
	porta		=  int(sys.argv[1])

	#pega o nome do server
	hostname	= socket.gethostname()
	print(f"Servidor ligado :)\nOlá\nMeu nome é {hostname}\nBom te ver, usuário\n\nAguardando mensagens...\n");

	#consegue o endreço IP do server
	hostIP = socket.gethostbyname(hostname)
	if (hostIP  == 0):
		print(f"Erro:	Não foi possível encontrar o endereço IP do servidor")
		exit (1)

	#pega o socket e dá bind
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.bind((hostIP, porta))

	while (True):
		#pega a primeira mensagem que possui o número de mensagens
		sock.settimeout(None)
		mensagem_bytes, addr	= sock.recvfrom(BUFFERSIZE)
		n_msg					= pickle.loads(mensagem_bytes)

		num_anterior	= -1
		chegou			= [0] * n_msg
		desord			= list()

		sock.settimeout(1)

		while (True):
			try:
				mensagem_bytes, addr	= sock.recvfrom(BUFFERSIZE)
				num_recebido			= pickle.loads(mensagem_bytes)

				if (num_recebido < num_anterior):
					desord.append(num_recebido)

				chegou[num_recebido]	= 1

				num_anterior			= num_recebido

			except socket.timeout:
				break
		
		print(f"O recebimento de {n_msg} mensagens acabou, esperando mais...")

		for i in range(0, n_msg):
			if chegou[num_recebido] == 0:
				print("A mensagem de número {i} não chegou\n")

		for i in range(0, len(desord)):
			print("A mensagem de número {i} chegou fora de ordem\n")

	exit(1)

