CC=gcc
CFLAGS=-Wall -g
LEX=flex
YACC=yacc

SRC=src/main.c src/lexer.l src/parser.y src/symtab.c src/ast.c src/codegen.c
OBJ=$(SRC:.c=.o)

all: compiler

compiler: $(OBJ)
	$(CC) -o compiler $(OBJ)

src/lexer.c: src/lexer.l
	$(LEX) -o src/lexer.c src/lexer.l

src/parser.c: src/parser.y
	$(YACC) -d -o src/parser.c src/parser.y

clean:
	rm -f src/*.o src/*.c src/*.h compiler
	rm -f src/parser.tab.c src/parser.tab.h src/lexer.c

.PHONY: all clean