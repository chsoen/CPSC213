CFLAGS += -std=gnu11 -g
EXES    = namelist

all:  $(EXES)
clean:
	-rm -f $(EXES)

namelist: namelist.c
# don't treat all and clean as file targets
.PHONY: all clean
