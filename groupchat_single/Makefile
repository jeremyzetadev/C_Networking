all: clientrun serverrun

clientrun: client.o socketutil.o
	gcc client.o socketutil.o -o clientrun

serverrun: server.o socketutil.o
	gcc server.o socketutil.o -o serverrun

socketutil.o: socketutil.c
	gcc -c socketutil.c

server.o: server.c socketutil.h
	gcc -c server.c

client.o: client.c socketutil.h
	gcc -c client.c

clean:
	rm *.o clientrun serverrun
