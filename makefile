PROGRAMS=cipher testlcg
CFLAGS= -Wall -ansi -pedantic -g

all: $(PROGRAMS)

cipher: cipher.c lcg.c lcg.h 
	gcc $(CFLAGS) -o cipher cipher.c lcg.c

testlcg: testlcg.c lcg.c lcg.h 
	gcc $(CFLAGS) -o testlcg testlcg.c lcg.c

clean:
	-rm $(PROGRAMS)


