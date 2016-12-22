#include <stdio.h>
#include <stdlib.h>
#include "DataType.h"
#include "symbolTable.h"

extern int yyparse();
extern FILE* yyin;
extern struct symTable *symbolTable;
int  main( int argc, char **argv )
{

	if( argc == 1 )
	{
		yyin = stdin;
	}
	else if( argc == 2 )
	{
		FILE *fp = fopen( argv[1], "r" );
		if( fp == NULL ) {
				fprintf( stderr, "Open file error\n" );
				exit(-1);
		}
		yyin = fp;
	}
	else
	{
	  	fprintf( stderr, "Usage: ./parser [filename]\n" );
   		exit(0);
 	} 
  symbolTable = (struct symTable*)malloc(sizeof(struct symTable));
  init_Table(symbolTable);

	yyparse();	/* primary procedure of parser */

	
	fprintf( stdout, "\n|--------------------------------|\n" );
	fprintf( stdout, "|  There is no syntactic error!  |\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	exit(0);
}

