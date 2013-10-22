#
# rdpp : road data preprocessor
# test : for test

CFLAGS	:= -Wall -pipe -O2
LDFLAGS	:= -lc

OBJS	= main.o process.o test.o ffind.o dummy.o

all: rdpp test

rdpp: main.o process.o ffind.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test: test.o dummy.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	$(RM) $(OBJS)
	$(RM) test.o

.PHONY: all clean
