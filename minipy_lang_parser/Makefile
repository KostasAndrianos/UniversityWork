all:
	bison -d parser.y
	flex lexical.l
	gcc -o parser parser.tab.c lex.yy.c
	./parser input.py>output.log
	make clean
clean:
	rm parser.tab.c parser.tab.h lex.yy.c parser
