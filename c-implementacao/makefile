CFLAGS = -Wall -g
OBJSS = servidor-udp.o
OBJSC = cliente-udp.o

all: servidor-udp cliente-udp

servidor-udp: $(OBJSS)
	gcc -o servidor-udp $(CFLAGS) $(OBJSS)

cliente-udp: $(OBJSC)
	gcc -o cliente-udp $(CFLAGS) $(OBJSC)

servidor-udp.o: servidor-udp.c
	gcc $(CFLAGS) -c servidor-udp.c

cliente-udp.o: cliente-udp.c
	gcc $(CFLAGS) -c cliente-udp.c

clean:
	-rm -f *~ *.o