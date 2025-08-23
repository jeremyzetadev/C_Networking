all: clientrun serverrun

# clientrun: client.o socketutil.o
# 	gcc -std=c11 client.o socketutil.o -o clientrun 
#
# serverrun: server.o socketutil.o
# 	gcc -std=c11 server.o socketutil.o -o serverrun -pthread
#
# socketutil.o: socketutil.c
# 	gcc -std=c11 -c socketutil.c
#
# server.o: server.c socketutil.h
# 	gcc -std=c11 -c server.c -pthread
#
# client.o: client.c socketutil.h
# 	gcc -std=c11 -c client.c

clientrun: client.o socketutil.o
	gcc client.o socketutil.o -o clientrun -lpthread -g

serverrun: server.o socketutil.o
	gcc server.o socketutil.o -o serverrun -lpthread -g

socketutil.o: socketutil.c
	gcc -c socketutil.c -lpthreead -g

server.o: server.c socketutil.h
	gcc -c server.c -lpthread -g

client.o: client.c socketutil.h
	gcc -c client.c -g


clean:
	rm *.o clientrun serverrun
