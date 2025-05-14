all: mini_compiler

mini_compiler: parser.tab.c lex.yy.c symtab.o
	gcc -o mini_compiler parser.tab.c lex.yy.c symtab.o -ll

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: scanner.l parser.tab.h
	flex scanner.l

symtab.o: symtab.c symtab.h
	gcc -c symtab.c

clean:
	rm -f mini_compiler parser.tab.c parser.tab.h lex.yy.c symtab.o