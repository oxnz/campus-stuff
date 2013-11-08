# RoadDataPreProcessor Project Makefile
# Version: 1.0
# Author: 0xnz
# Mail: yunxinyi@gmail.com
# Copying: Copyright (C) 2013, All rights reserved.
#
# Revision: -*-
# Last-updated: 2013-11-08 15:09:01 CST
#


CC	:= gcc
CXX	:= g++
CFLAGS	:= -Wall -pipe -O2
CXXFLAGS:= -Wall -pipe -O2 -std=c++0x -p
LDFLAGS	:= -p
VERSION := 1.1

HEADERS = RConstant.h \
		  RDPool.h \
		  RHelper.h \
		  RProcessor.h \
		  RTypes.h \
		  RsidGen.h \
		  NZLogger.h

OBJS	= main.o \
		  RDPool.o \
		  RProcessor.o \
		  RTest.o \
		  RHelper.o \
		  RsidGen.o \
		  test.o \
		  NZLogger.o

TARGETS = rdpp \
		  test

# rdpp : road data preprocessor
# test : test case

all: $(TARGETS)

rdpp: main.o RProcessor.o RHelper.o RsidGen.o RDPool.o NZLogger.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

test: RTest.o RHelper.o RsidGen.o NZLogger.o RDPool.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ -o $@

check: test
	./test

dist: clean
	tar --exclude="out"	\
		--create --gzip --verbose \
		--file `pwd`.tar.gz *

main.o: main.cpp RProcessor.h NZLogger.h
RTest.o: $(HEADERS)
RDPool.o: RDPool.cpp RDPool.h
RProcessor.o: RProcessor.cpp $(HEADERS)

clean:
	$(RM) $(OBJS)
	$(RM) $(TARGETS)
	$(RM) 201211*.dat gmon.out

.PHONY: all clean
