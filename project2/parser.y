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
%token int_ float_ scien str
%%

program : decl_and_def_list
        |
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

funct_def : type identifier L_PAREN arg_list R_PAREN compound_state
          | VOID identifier L_PAREN arg_list R_PAREN compound_state
          | type identifier L_PAREN R_PAREN compound_state
          | VOID identifier L_PAREN R_PAREN compound_state
    ;

const_decl : CONST type const_init_list SEMICOLON;

const_init_list : const_init_list COMMA const_init
                | const_init
    ;

const_init : identifier ASSIGN literal
    ;

var_decl : type var_init_list SEMICOLON;

var_init_list : var_init_list COMMA var_init
              | var_init_list COMMA arr_init
              | var_init
              | arr_init
    ;

var_init : identifier 
         | identifier ASSIGN operand
    ;

arr_init : arr
         | arr ASSIGN set 
    ;

arr : identifier dim_list
    ;

dim_list : dim_list L_BRACKET integer_const R_BRACKET
         | L_BRACKET integer_const R_BRACKET
    ;

set : L_BRACE R_BRACE
    | L_BRACE set_inner_list R_BRACE
    ;

set_inner_list : set_inner_list COMMA set_inner
               | set_inner
    ;

set_inner : expre
          | literal
          | set
    ;

expre_list : expre_list COMMA expre      
           | expre_list COMMA operand
           | expre
           | operand 
    ;

arg_list : arg_list COMMA type identifier
         | arg_list COMMA type arr
         | type identifier
         | type arr
    ;

funct_invo : identifier L_PAREN expre_list R_PAREN;

type : INT
     | FLOAT
     | DOUBLE
     | STRING
     | BOOL
    ;

state_list : state_list state
           | state
    ;

state : compound_state
      | sim_state
      | cond_state
      | while_state
      | for_state
      | jump_state
    ;


compound_state : L_BRACE compound_body R_BRACE
    ;

compound_body : var_const_decl_list state_list
              | var_const_decl_list
              | state_list
              |
    ;
    

sim_state : var_assign SEMICOLON
          | PRINT var_ref SEMICOLON
          | PRINT operand SEMICOLON
          | READ var_ref SEMICOLON
          | funct_invo SEMICOLON
    ;

var_assign : var_ref ASSIGN expre
           | var_ref ASSIGN literal
    ;

cond_state : IF L_PAREN operand R_PAREN compound_state ELSE compound_state
           | IF L_PAREN operand R_PAREN compound_state

while_state : WHILE L_PAREN operand R_PAREN compound_state
            | DO compound_state WHILE L_PAREN operand R_PAREN SEMICOLON
    ;

for_state : FOR L_PAREN for_expre R_PAREN compound_state
    ;

for_expre : SEMICOLON SEMICOLON
          | SEMICOLON SEMICOLON set_expre
          | SEMICOLON operand SEMICOLON
          | set_expre SEMICOLON SEMICOLON 
          | SEMICOLON operand SEMICOLON set_expre
          | set_expre SEMICOLON SEMICOLON set_expre
          | set_expre SEMICOLON operand SEMICOLON 
          | set_expre SEMICOLON operand SEMICOLON set_expre
    ; 

set_expre : funct_invo
          | var_assign
    ;

jump_state : RETURN expre SEMICOLON
           | RETURN literal SEMICOLON 
           | BREAK SEMICOLON
           | CONTINUE SEMICOLON
    ;

expre : bool_expre
      | arith_expre
      | L_PAREN expre R_PAREN
    ;

arith_expre : operand PLUS operand
            | operand SUB operand
            | operand MUL operand
            | operand DIV operand
            | operand MOD operand
            | L_PAREN operand R_PAREN
    ;

bool_expre : operand OR operand
           | operand AND operand
           | NOT operand 
           | operand LT operand
           | operand LE operand
           | operand EQ operand
           | operand GE operand
           | operand GT operand
           | operand NE operand
    ;
    
operand : expre
        | var_ref
        | literal
        | funct_invo
    ;

var_ref : identifier
        | arr_inst
    ;

arr_inst : identifier inst_list
    ;

inst_list : inst_list L_BRACKET operand R_BRACKET
          | L_BRACKET operand R_BRACKET
    ;

identifier : ID
	  ;	

literal : SUB num
        | num
        | str
    ;

num : integer_const
    | float_
    | scien
    | TRUE
    | FALSE
    ;

integer_const : int_
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

