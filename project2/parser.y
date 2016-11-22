%{
#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */
%}

%token SEMICOLON    /* ; */
%token ID           /* identifier */
%token INT          /* keyword */
%token COMMA L_PAREN R_PAREN L_BRACKET R_BRACKET L_BRACE R_BRACE /* delimiter */
%left PLUS SUB MUL DIV MOD ASSIGN LT LE NE GE GT EQ AND OR
%right NOT
%token WHILE DO IF ELSE TRUE FALSE FOR PRINT CONST READ BOOLEAN BOOL VOID FLOAT DOUBLE STRING CONTINUE BREAK RETURN
%token int float scien str
%%

program : decl_and_def_list
	  ;

decl_and_def_list	: decl_and_def_list declaration
			            | decl_and_def_list definition
                  | declaration
                  | definition
    ;

declaration : var_decl
            | const_decl
            | funct_decl
		;

definition : funct_def
    ;

var_const_decl_list : var_const_decl_list var_decl
                    | var_const_decl_list const_decl
                    | var_decl
                    | const_decl
    ;

funct_decl : type identifier L_PAREN arg_list R_PAREN SEMICOLON
           | VOID identifier L_PAREN arg_list R_PAREN SEMICOLON
           | type identifier L_PAREN R_PAREN SEMICOLON
           | VOID identifier L_PAREN R_PAREN SEMICOLON
    ;

funct_def : type identifier L_PAREN arg_list R_PAREN compound_statement
          | VOID identifier L_PAREN arg_list R_PAREN compound_statement
          | type identifier L_PAREN R_PAREN compound_statement
          | VOID identifier L_PAREN R_PAREN compound_statement
    ;

var_decl : type id_decl_list SEMICOLON
    ;

const_decl : CONST type id_assign_list SEMICOLON
    ;

arg_list : arg_list arg
         | arg
    ;

id_decl_list : id_decl_list COMMA identifier
             | id_decl_list COMMA id_assign_list
             | id_decl_list COMMA array_assign_list
             | identifier
             | id_assign_list
             | array_assign_list
    ;

id_assign_list : id_assign_list COMMA id_assign
               | id_assign
    ;

array_assign_list : identifier dim_list ASSIGN set
    ;

arg : type identifier
    | type identifier dim_list
    ;

id_assign : identifier ASSIGN literal
          | identifier ASSIGN simple_statement
    ;

type : INT
     | FLOAT
     | DOUBLE
     | STRING
     | BOOL
    ; 

identifier : ID
	  ;	

literal : SUB num
        | num
        | str
    ;

num : integer_const
    | float
    | scien
    | TRUE
    | FALSE
    ;

integer_const : int
    ;

dim_list : dim_list dim
         | dim
    ;

dim : L_BRACKET integer_const R_BRACKET
    | L_BRACKET identifier R_BRACKET
    | L_BRACKET simple_statement R_BRACKET
    ;

set : set L_BRACE simple_statement R_BRACE
    | set L_BRACE literal R_BRACE
    | L_BRACE simple_statement R_BRACE
    | L_BRACE literal R_BRACE
    ;

statement_list : statement_list statement
               | statement
    ;

statement : compound_statement
          | simple_statement
          | conditional_statement
          | while_statement
          | for_statement
          | jump_statement
    ;

compound_statement : L_BRACE var_const_decl_list statement_list  R_BRACE
                   | L_BRACE statement_list R_BRACE
                   | L_BRACE R_BRACE
    ;

simple_statement : var_assign SEMICOLON
                 | PRINT var_ref SEMICOLON
                 | PRINT simple_statement SEMICOLON
                 | READ var_ref SEMICOLON
                 | expression SEMICOLON
                 | funct_invocation SEMICOLON
    ;

conditional_statement : IF L_PAREN boolean_expression R_PAREN compound_statement ELSE compound_statement
                      | IF L_PAREN boolean_expression R_PAREN compound_statement
    ;

while_statement : WHILE L_PAREN boolean_expression R_PAREN compound_statement
                | DO compound_statement WHILE L_PAREN boolean_expression R_PAREN SEMICOLON
    ;

for_statement : FOR L_PAREN set_expre SEMICOLON boolean_expression SEMICOLON set_expre R_PAREN compound_statement
              | FOR L_PAREN SEMICOLON boolean_expression SEMICOLON set_expre R_PAREN compound_statement
              | FOR L_PAREN set_expre SEMICOLON SEMICOLON set_expre R_PAREN compound_statement
              | FOR L_PAREN set_expre SEMICOLON boolean_expression SEMICOLON R_PAREN compound_statement
              | FOR L_PAREN SEMICOLON SEMICOLON set_expre R_PAREN compound_statement
              | FOR L_PAREN SEMICOLON boolean_expression SEMICOLON R_PAREN compound_statement 
              | FOR L_PAREN set_expre SEMICOLON SEMICOLON R_PAREN compound_statement
              | FOR L_PAREN SEMICOLON SEMICOLON R_PAREN compound_statement
    ;

jump_statement : RETURN simple_statement SEMICOLON
               | BREAK SEMICOLON
               | CONTINUE SEMICOLON
    ;

var_ref : identifier dim_list
        | identifier
    ;

var_assign : var_ref ASSIGN simple_statement
           | id_assign
    ;

expression : boolean_expression
           | arith_expression
    ;

funct_invocation : identifier L_PAREN expression_list R_PAREN 
    ;

boolean_expression : set_expre OR set_expre
                   | set_expre AND set_expre
                   | NOT set_expre
                   | set_expre LT set_expre
                   | set_expre LE set_expre
                   | set_expre EQ set_expre
                   | set_expre GE set_expre
                   | set_expre GT set_expre
                   | set_expre NE set_expre
    ;

arith_expression : set_expre PLUS set_expre
                 | set_expre SUB set_expre
                 | set_expre MUL set_expre
                 | set_expre DIV set_expre
                 | set_expre MOD set_expre
                 | SUB set_expre %prec MOD
    ;

set_expre : expression
          | var_assign
          | funct_invocation
    ;

expression_list : expression_list COMMA expression
                | expression
    ;

%%

int yyerror( char *msg )
{
  fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
  fprintf( stderr, "|--------------------------------------------------------------------------\n" );
  exit(-1);
}

int  main( int argc, char **argv )
{
	if( argc != 2 ) {
		fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
		exit(0);
	}

	FILE *fp = fopen( argv[1], "r" );
	
	if( fp == NULL )  {
		fprintf( stdout, "Open  file  error\n" );
		exit(-1);
	}
	
	yyin = fp;
	yyparse();

	fprintf( stdout, "\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	fprintf( stdout, "|  There is no syntactic error!  |\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	exit(0);
}

