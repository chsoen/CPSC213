CC=gcc
CFLAGS=-g -std=gnu11
LDFLAGS=-pthread
LDLIBS=-lcrypt
OBJS=threadpool.o uthread.o uthread_mutex_cond.o uthread_sem.o

all: main bruteforce encrypt

main: main.o $(OBJS)
bruteforce: bruteforce.o $(OBJS) $(LDLIBS)
encrypt: encrypt.o

clean:
	-rm -rf encrypt encrypt.o main main.o bruteforce bruteforce.o $(OBJS)
tidy: clean
	-rm -rf *~

.PHONY: all clean tidy
