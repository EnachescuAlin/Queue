CFLAGS = -std=c99 -Wall -c
INCLUDE = -I../include

all: queue.o libqueue.a libqueue.la

queue.o: queue.c
	mkdir -p ../bin
	gcc $(CFLAGS) -o ../bin/queue.o queue.c $(INCLUDE)

libqueue.a:
	ar rcs ../bin/libqueue.a ../bin/queue.o

libqueue.la:
	ar rcs ../bin/libqueue.la ../bin/queue.o

clean:
	rm ../bin/queue.o ../bin/libqueue.a ../bin/libqueue.la
	rm -r ../bin
