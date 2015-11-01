all:test.exe parse.exe test1 test2

test2:
	cat output.txt | ./parse.exe > finalo.s

test1:
	cat A_sample1.c | ./test.exe > output.txt

parse.exe: parse.cpp
	g++ parse.cpp -o parse.exe
	
test.exe: y.tab.o lex.yy.o util.o
	g++ y.tab.o lex.yy.o util.o -o test.exe

y.tab.o lex.yy.o: y.tab.c lex.yy.c util.cpp util.h error.h
	g++ -c y.tab.c lex.yy.c util.cpp

lex.yy.c: scanner.l y.tab.h
	flex scanner.l

y.tab.c: parser.y
	bison -y -d parser.y

clean :
	rm lex.yy.c lex.yy.o test.exe y.tab.c y.tab.h y.tab.o util.o
