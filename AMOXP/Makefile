all:	\
	chapter1 chapter2 chapter4 chapter11 chapter12 \
	chapter13 chapter14 chapter20 chapter23
chapter1:	\
	expansion stringization conditional-compilation predef multilineMacro \
	vararg sentinel debuglog describeObjects quietlog debuglog-macro \
	bitmask datasource
chapter2:	\
	warning sizeprinter
chapter4:	\
	upcase downcase dumpargs argparse defargs open longjmp
chapter11:	\
	append writestring readstring writevecstring readvecstring buffread \
	fbinaryio tempfun uid mmap-rot13
chapter12:	\
	cheapls permtype
chapter13:	\
       searchpath urldirectory urlmeta urlenumerator filerefurl
chapter14:	\
	ptontoa resolve simpleserver simpleclient
chapter20:	\
	basics mutex
chapter23:	\
	dumpem

# chapter 1
expansion: expansion.m
	gcc -g -Wall -o $@ $<
stringization: stringization.m
	gcc -g -Wall -o $@ $<
conditional-compilation: conditional-compilation.m
	gcc -g -Wall -o $@ $<
predef: predef.mm
	g++ -g -Wall -o $@ -framework Foundation $<
multilineMacro: multilineMacro.m
	gcc -g -Wall -o $@ $<
vararg: vararg.m
	gcc -g -Wall -o $@ $<
sentinel: sentinel.m
	gcc -g -Wall -o $@ $<
debuglog: debuglog.m
	gcc -g -Wall -o $@ $<
describeObjects: describeObjects.m
	gcc -g -Wall -o $@ -framework Foundation  $<
quietlog: quietlog.m
	gcc -g -Wall -o $@ -framework Foundation  $<
debuglog-macro: debuglog-macro.m
	gcc -g -Wall -o $@ $<
bitmask: bitmask.m
	gcc -g -Wall -o $@ $<
datasource: datasource.m
	gcc -g -Wall -o $@ -framework Foundation $<
# chapter 2
warning: warning.m
	gcc -Wall -Wno-unused -o $@ $<
sizeprinter: sizeprinter.c
	gcc -arch i386 -arch x86_64 -g -Wall -o $@ $<
# chapter 4
upcase: upcase.m
	gcc -g -Wall -o $@ -framework Foundation $<
downcase: upcase
	ln -s upcase downcase
dumpargs: dumpargs.m
	gcc -g -std=c99 -Wall -o $@ $<
argparse: argparse.m
	gcc -g -std=c99 -Wall -o $@ $<
defargs: defargs.m
	gcc -g -Wall -o $@ -framework Foundation $<
open: open.m
	gcc -g -Wmost -o $@ $<
longjmp: longjmp.m
	gcc -g -Wall -o $@ $<
# chapter 11
append: append.m
	gcc -g -Wall -o $@ $<
writestring: writestring.m
	gcc -g -Wall -o $@ $<
readstring: readstring.m
	gcc -g -Wall -o $@ $<
writevecstring: writevecstring.m
	gcc -g -Wall -o $@ $<
readvecstring: readvecstring.m
	gcc -g -Wall -o $@ $<
buffread: buffread.m
	gcc -g -Wall -o $@ $<
fbinaryio: fbinaryio.m
	gcc -g -std=c99 -Wall -o $@ $<
tempfun: tempfun.m
	gcc -g -Wall -framework Foundation -o $@ $<
uid: uid.m
	gcc -g -Wall -o $@ $<
mmap-rot13: mmap-rot13.m
	gcc -g -std=c99 -Wall -o $@ $<
# chapter 12
cheapls: cheapls.m
	gcc -g -Wall -Wextra -o $@ $<
permtype: permtype.m
	clang -g -std=c99 -Wall -Wextra -o $@ $<
flodersize: foldersize.m
	gcc -g -Wall -Wextra -o $@ $<
# chapter 13
searchpath: searchpath.m
	gcc -g -Wall -framework Foundation -o $@ $<
urldirectory: urldirectory.m
	gcc -g -Wall -framework Foundation -o $@ $<
urlmeta: urlmeta.m
	gcc -g -Wall -framework Foundation -o $@ $<
urlenumerator: urlenumerator.m
	clang -g -std=c99 -Wall -framework Foundation -o $@ $<
filerefurl: filerefurl.m
	gcc -g -Wall -framework Foundation -o $@ $<
# chapter 14
ptontoa: ptontoa.m
	gcc -g -Wall -o $@ $<
resolv: resolv.m
	gcc -std=c99 -g -Wall -o $@ $<
simpleserver: simpleserver.m
	gcc -std=c99 -g -Wall -o $@ $<
simpleclient: simpleclient.m
	gcc -std=c99 -g -Wall -o $@ $<
#chapter 20
basics: basics.m
	gcc -std=c99 -g -Wall -o $@ $<
mutex: mutex.m
	gcc -std=c99 -g -Wall -o $@ $<
#chapter 23
dumpem: dumpem.m
	gcc -std=c99 -g -Wall -framework Security -framework Foundation -o $@ $<


.PHONY: all clean infodump
infodump:
	gcc -E -dM - < /dev/null
	clang -E -dM - < /dev/null

clean:
	rm -rf *.dSYM
