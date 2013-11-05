# RoadDataPreProcessor Project Makefile
# Version: 0.1
# Author: 0xnz
# Revision: -*-
# Last-updated: 2013-11-05 15:20:00 CST
#


CC	:= clang
CXX	:= clang++
CFLAGS	:= -Wall -pipe -O2
CXXFLAGS:= -Wall -pipe -O2
LDFLAGS	:=

OBJS	= main.o test.o RProcessor.o RTest.o RHelper.o
TARGETS = rdpp test

# rdpp : road data preprocessor
# test : test case

all: $(TARGETS)

rdpp: main.o RProcessor.o RHelper.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

test: RTest.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

RProcessor.o RTest.o: RsidGen.h
main.o: main.cpp RProcessor.h RTypes.h RConstant.h
RProcessor.o: RProcessor.cpp RProcessor.h RTypes.h RConstant.h

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGETS)

.PHONY: all clean
