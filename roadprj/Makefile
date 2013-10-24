#
# rdpp : road data preprocessor
# test : for test
CXX	:= g++
CFLAGS	:= -Wall -pipe -O2
LDFLAGS	:= -lc

OBJS	= main.o processor.o test.o ffind.o dummy.o

all: rdpp test

rdpp: main.o processor.o dummy.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

test: test.o dummy.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.cpp processor.h types.h
processor.o: processor.cpp processor.h types.h
clean:
	$(RM) $(OBJS)
	$(RM) test.o

.PHONY: all clean
