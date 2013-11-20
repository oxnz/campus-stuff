AM_CXXFLAGS = -Wall -Werror -std=c++11 -I$(top_srcdir)/include
AM_YFLAGS = -d -p mfg
AM_LFALGS = -o $(LEX_OUTPUT_ROOT).c

bin_PROGRAMS = microcc
noinst_PROGRAMS = test

microcc_SOURCES = main.cpp \
				  microcc.cpp \
				  token.cpp \
				  preproc.cpp \
				  lexical.cpp \
				  syntax.cpp \
				  semantic.cpp \
				  midcode.cpp \
				  optimize.cpp \
				  codegen.cpp

test_SOURCES = test.cpp
