#
# rdpp : road data preprocessor
# test : for test

CFLAGS	:= -Wall -pipe -O2
LDFLAGS	:=

OBJS	= main.o process.o test.o

all: rdpp test

rdpp: main.o process.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test: test.o dummy.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	$(RM) $(OBJS)
	$(RM) test.o

.PHONY: all clean
