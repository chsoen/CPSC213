CFLAGS  += -std=gnu11 -g
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
LDFLAGS += -pthread
endif
EXES = q1 q2 q3
OBJS = uthread.o uthread_mutex_cond.o

all: $(EXES)

$(EXES): $(OBJS)

clean:
	rm -f *.o $(EXES); rm -rf *.dSYM
tidy:
	rm -f *.o; rm -rf *.dSYM