CFLAGS = -Wall -g
OBJSS = servidor-udp.o
OBJSC = cliente-udp.o

all: servidor-udp cliente-udp

servidor-udp: $(OBJSS)
	mpic++ -o servidor-udp $(CFLAGS) $(OBJSS)

cliente-udp: $(OBJSC)
	mpic++ -o cliente-udp $(CFLAGS) $(OBJSC)

servidor-udp.o: servidor-udp.c
	mpic++ $(CFLAGS) -c servidor-udp.c

cliente-udp.o: cliente-udp.c
	mpic++ $(CFLAGS) -c cliente-udp.c

clean:
	-rm -f *~ *.o