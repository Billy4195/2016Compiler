all: parser

parser.tab.c: parser.y
	bison -d parser.y

scanner.yy.c: scanner.l
	flex -o scanner.yy.c scanner.l

parser.tab.h: parser.y
	bison -d parser.y

parser: scanner.yy.c parser.tab.c parser.tab.h
	gcc parser.tab.c scanner.yy.c -o parser -ly -ll
