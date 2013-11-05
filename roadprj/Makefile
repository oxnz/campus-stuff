# RoadDataPreProcessor Project Makefile
# Version: 0.1
# Author: 0xnz
# Revision: -*-
# Last-updated: 2013-11-05 11:26:06 CST
#


CC	:= clang
CXX	:= clang++
CFLAGS	:= -Wall -pipe -O2
CXXFLAGS	:= -Wall -pipe -O2
LDFLAGS	:= -lc

OBJS	= main.o RProcessor.o RTest.o RHelper.o

# rdpp : road data preprocessor
# test : for test
all: rdpp test
RProcessor.o RTest.o: RsidGen.h
rdpp: main.o RProcessor.o RHelper.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

test: RTest.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.cpp RProcessor.h RTypes.h RConstant.h
RProcessor.o: RProcessor.cpp RProcessor.h RTypes.h RConstant.h
clean:
	$(RM) $(OBJS)
	$(RM) test.o
.PHONY: all clean
