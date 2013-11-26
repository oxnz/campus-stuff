AM_CXXFLAGS = -Wall -Werror -std=c++11
#AM_CXXFLAGS = -Wall -std=c++11

bin_PROGRAMS = rdpp
noinst_PROGRAMS = test

rdpp_SOURCES = main.cpp \
			   RExcept.h \
			   RProcessor.cpp \
			   RHelper.cpp \
			   RDPool.cpp \
			   RProcessor.h \
			   RHelper.h \
			   RDPool.h \
			   RsidGen.h \
			   RTypes.h \
			   RConstant.h \
			   NZUtils.h

rdpp_LDADD = ../libnz/libnz.a

test_SOURCES = RTest.cpp \
			   RHelper.h \
			   RHelper.cpp \
			   RDPool.h \
			   RDPool.cpp \
			   RsidGen.h

test_LDADD = ../libnz/libnz.a
