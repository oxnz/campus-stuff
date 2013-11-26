AM_CPPFLAGS = -std=c++11

noinst_LIBRARIES = libnz.a

noinst_PROGRAMS = test

libnz_a_SOURCES = NZUtils.h \
			   NZExcept.h \
			   NZFinder.h \
			   NZFinder.cpp \
			   NZLogger.h \
			   NZLogger.cpp

test_SOURCES = test.cpp \
			   NZUtils.h \
			   NZFinder.h \
			   NZExcept.h \
			   NZLogger.h

test_LDADD = libnz.a
