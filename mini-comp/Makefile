CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lfl

all: minicompiler

minicompiler: y.tab.o lex.yy.o ast.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

y.tab.c y.tab.h: parser.y
	yacc -d parser.y

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

clean:
	rm -f minicompiler *.o y.tab.c y.tab.h lex.yy.c