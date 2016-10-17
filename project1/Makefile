all: scanner 

scanner.yy.c: scanner.l
	flex -o scanner.yy.c scanner.l
scanner: scanner.yy.c
	gcc scanner.yy.c -o scanner -lfl
