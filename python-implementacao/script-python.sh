make -B 2> /dev/null

#para re-fazer os experimentos, excluímos os arquivos antigos
#caso não queira excluir, guarde os arquivos antigos em outro diretório
rm py-nchegou-100.txt py-nchegou-1000.txt py-nchegou-10000.txt py-nchegou-100000.txt py-nchegou-1000000.txt 2> /dev/null

#alterar esse valor para o nome do servidor
NOMESERV=h23
PORTA=1616

# para bloqueantes
for NMSG in 100 1000 10000 100000 1000000
do
    python3 ./python-implementacao/cliente-udp.py "$NOMESERV" "$PORTA" "$NMSG"
done