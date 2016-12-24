%{
#include <stdio.h>
#include <stdlib.h>
#include "DataType.h"
#include "symbolTable.h"
#include "check.h"


extern int linenum;
extern FILE	*yyin;
extern char	*yytext;
extern char buf[256];
int error_happened;
int level = 0;
struct symTable *symbolTable;

%}
%union{
    int intval;
    float floval;
    double douval;
    char *strval;
    struct ConstAttr *constAttr;
    struct Dim *dim;
    struct Type *type;
    struct ID_type *id;
    struct ID_list *id_list;
    struct Param_list *param_list;
    struct Const_list *const_list;
}

%type <constAttr> literal_const
%type <type> scalar_type
%type <dim> dim
%type <id> array_decl
%type <id_list> identifier_list
%type <param_list> parameter_list
%type <const_list> const_list

%token	<strval> ID
%token	<intval> INT_CONST
%token	<douval> FLOAT_CONST
%token	<douval> SCIENTIFIC
%token	<strval> STR_CONST

%token	LE_OP
%token	NE_OP
%token	GE_OP
%token	EQ_OP
%token	AND_OP
%token	OR_OP

%token	READ
%token	BOOLEAN
%token	WHILE
%token	DO
%token	IF
%token	ELSE
%token	TRUE
%token	FALSE
%token	FOR
%token	INT
%token	PRINT
%token	BOOL
%token	VOID
%token	FLOAT
%token	DOUBLE
%token	STRING
%token	CONTINUE
%token	BREAK
%token	RETURN
%token  CONST

%token	L_PAREN
%token	R_PAREN
%token	COMMA
%token	SEMICOLON
%token	ML_BRACE
%token	MR_BRACE
%token	L_BRACE
%token	R_BRACE
%token	ADD_OP
%token	SUB_OP
%token	MUL_OP
%token	DIV_OP
%token	MOD_OP
%token	ASSIGN_OP
%token	LT_OP
%token	GT_OP
%token	NOT_OP

/*	Program 
	Function 
	Array 
	Const 
	IF 
	ELSE 
	RETURN 
	FOR 
	WHILE
*/
%start program
%%

program :  decl_list funct_def decl_and_def_list {
    print_Table(symbolTable,0);
    check_Func_define(symbolTable);
}
           ;

decl_list : decl_list var_decl
		  | decl_list const_decl
		  | decl_list funct_decl
		  |
		  ;


decl_and_def_list : decl_and_def_list var_decl
				  | decl_and_def_list const_decl
				  | decl_and_def_list funct_decl
				  | decl_and_def_list funct_def
				  | 
				  ;

funct_def : scalar_type ID L_PAREN R_PAREN {
    struct symEntry *new_node;
    struct symEntry *node = find_ID_Decl(symbolTable, $2);
    if(node){
        int ret = check_Func_Type_and_Param_equal(node,$2,$1,NULL);
        if(ret == 0){
            node->isDef = __TRUE;
        }
    }else{
        new_node = createFunc_node($1, $2, NULL, level, __TRUE);
        Table_push_back(symbolTable,new_node);
    }
}
      compound_statement 
		  | scalar_type ID L_PAREN parameter_list R_PAREN { 
    struct symEntry *new_node;
    struct symEntry *node = find_ID_Decl(symbolTable, $2);
    if(node){
        int ret = check_Func_Type_and_Param_equal(node,$2,$1,$4);
        if(ret == 0){
            node->isDef = __TRUE;
        }
    }else{
        new_node = createFunc_node($1, $2, $4, level, __TRUE);
        Table_push_back(symbolTable,new_node);
    }
    struct Param *it = $4->head;
    for(;it != NULL; it = it->next){
        node = find_ID_Decl(symbolTable,it->name);
        if(node){
            Name_reuse(it->name);
        }else{
            new_node = createParam_node(it, level+1);
            Table_push_back(symbolTable, new_node);
        }
    }
}
      compound_statement 
		  | VOID ID L_PAREN R_PAREN {
    struct symEntry *new_node;
    struct symEntry *node = find_ID_Decl(symbolTable, $2);
    if(node){
        int ret = check_Func_Type_and_Param_equal(node,$2,new_Type(VOID_t),NULL);
        if(ret == 0){
            node->isDef = __TRUE;
        }
    }else{
        new_node = createFunc_node(new_Type(VOID_t), $2, NULL, level, __TRUE);
        Table_push_back(symbolTable,new_node);
    }
}
      compound_statement 
		  | VOID ID L_PAREN parameter_list R_PAREN {
    struct symEntry *new_node;
    struct symEntry *node = find_ID_Decl(symbolTable, $2);
    if(node){
        int ret = check_Func_Type_and_Param_equal(node,$2,new_Type(VOID_t),$4);
        if(ret == 0){
            node->isDef = __TRUE;
        }
    }else{
        new_node = createFunc_node(new_Type(VOID_t), $2, $4, level, __TRUE);
        Table_push_back(symbolTable,new_node);
    }
    struct Param *it = $4->head;
    for(;it != NULL; it = it->next){
        node = find_ID_Decl(symbolTable,it->name);
        if(node){
            Name_reuse(it->name);
        }else{
            new_node = createParam_node(it, level+1);
            Table_push_back(symbolTable, new_node);
        }
    }
}
      compound_statement 
		  ;

funct_decl : scalar_type ID L_PAREN R_PAREN SEMICOLON {
    struct symEntry *node = find_ID_Decl(symbolTable, $2);
    if(node){
        Redeclaration($2);
        delete_Type($1);
    }else{
        struct symEntry *new_node = createFunc_node($1, $2, NULL, level, __FALSE);
        Table_push_back(symbolTable,new_node);
    }
}
	 	   | scalar_type ID L_PAREN parameter_list R_PAREN SEMICOLON {
    struct symEntry *node = find_ID_Decl(symbolTable, $2);
    if(node){
        Redeclaration($2);
        delete_Type($1);
        delete_Param_list($4);
    }else{
        struct symEntry *new_node = createFunc_node($1, $2, $4, level, __FALSE);
        Table_push_back(symbolTable,new_node);
    }
}
		   | VOID ID L_PAREN R_PAREN SEMICOLON {
    struct symEntry *node = find_ID_Decl(symbolTable, $2);
    if(node){
        Redeclaration($2);
    }else{
        struct symEntry *new_node = createFunc_node(new_Type(VOID_t), $2, NULL, level, __FALSE);
        Table_push_back(symbolTable,new_node);
    }
}
		   | VOID ID L_PAREN parameter_list R_PAREN SEMICOLON {
    struct symEntry *node = find_ID_Decl(symbolTable, $2);
    if(node){
        Redeclaration($2);
        delete_Param_list($4);
    }else{
        struct symEntry *new_node = createFunc_node(new_Type(VOID_t), $2, $4, level, __FALSE);
        Table_push_back(symbolTable,new_node);
    }
}
		   ;

parameter_list : parameter_list COMMA scalar_type ID {
    Param_list_push_back($1, new_Param($3, new_ID($4, NULL)));
    $$ = $1;
}
			   | parameter_list COMMA scalar_type array_decl {
    Param_list_push_back($1, new_Param($3, $4));
    $$ = $1;
}
			   | scalar_type array_decl {
    $$ = new_Param_list( new_Param($1, $2) );
}
			   | scalar_type ID {
    $$ = new_Param_list( new_Param($1, new_ID($2, NULL)) );
}
			   ;

var_decl : scalar_type identifier_list SEMICOLON {
    struct ID_type *it = $2->head,*tmp;
    struct symEntry *new_node;
    struct symEntry *node;
    for(;it != NULL ;it = tmp){
        node = find_ID_Decl(symbolTable,it->name);
        if(node){
            Name_reuse(it->name);
        }else{
            new_node = createVar_node($1, it, level);
            Table_push_back(symbolTable, new_node);
            tmp = it->next;
            free(it);
        }
    }
}
		 ;

identifier_list : identifier_list COMMA ID {
    ID_list_push_back($1, new_ID($3, NULL));
    $$ = $1;
}
		 		| identifier_list COMMA ID ASSIGN_OP logical_expression {
    ID_list_push_back($1, new_ID($3, NULL));
    $$ = $1;
}
				| identifier_list COMMA array_decl ASSIGN_OP initial_array {
    ID_list_push_back($1, $3);
    $$ = $1;
}
				| identifier_list COMMA array_decl {
    ID_list_push_back($1, $3);
    $$ = $1;
}
				| array_decl ASSIGN_OP initial_array {
    $$ = new_ID_list( $1 );
}
				| array_decl {
    $$ = new_ID_list( $1 );
}
				| ID ASSIGN_OP logical_expression {
    $$ = new_ID_list( new_ID($1,NULL) );
}
				| ID {
    $$ = new_ID_list( new_ID($1,NULL) );
}
				;

initial_array : L_BRACE literal_list R_BRACE
			  ;

literal_list : literal_list COMMA logical_expression
			 | logical_expression
                         | 
			 ;

const_decl : CONST scalar_type const_list SEMICOLON {
    struct Const_type *it = $3->head,*tmp;
    struct symEntry *new_node;
    struct symEntry *node;
    for(;it != NULL ;it = tmp){
        node = find_ID_Decl(symbolTable,it->name);
        if(node){
            Name_reuse(it->name);
        }else{
            new_node = createConst_node($2, it, level);
            Table_push_back(symbolTable, new_node);
            tmp = it->next;
            free(it);
        }
    }
}
       ;

const_list : const_list COMMA ID ASSIGN_OP literal_const{
    Const_list_push_back($1, new_Const($3, $5));
    $$ = $1;
}
		   | ID ASSIGN_OP literal_const {
    $$ = new_Const_list( new_Const($1, $3) );
}
                    
		   ;

array_decl : ID dim {
    if($2 == NULL){
        Array_index_error($1);
        $$ = NULL;
    }else{
        $$ = new_ID($1,$2);
    }
}
		   ;

dim : dim ML_BRACE INT_CONST MR_BRACE {
    if($1 == NULL || $3 == 0){
        $$ = NULL;
    }else{
        Dim_add_new_num($1,$3);
        $$ = $1;
    }
}
	| ML_BRACE INT_CONST MR_BRACE {
    if($2 == 0){
        $$ = NULL;
    }else{
        $$ = new_Dim($2);
    }
}
	;

compound_statement : { level++; } L_BRACE var_const_stmt_list R_BRACE {
    print_Table(symbolTable, level);
    Table_pop_back(symbolTable, level);
    level -= 1;
}
				   ;

var_const_stmt_list : var_const_stmt_list statement	
				    | var_const_stmt_list var_decl
					| var_const_stmt_list const_decl
				    |
				    ;

statement : compound_statement
		  | simple_statement
		  | conditional_statement
		  | while_statement
		  | for_statement
		  | function_invoke_statement
		  | jump_statement
		  ;		

simple_statement : variable_reference ASSIGN_OP logical_expression SEMICOLON
				 | PRINT logical_expression SEMICOLON
				 | READ variable_reference SEMICOLON
				 ;

conditional_statement : IF L_PAREN logical_expression R_PAREN compound_statement
					  | IF L_PAREN logical_expression R_PAREN compound_statement
						ELSE
              compound_statement
					  ;
while_statement : WHILE L_PAREN logical_expression R_PAREN compound_statement
				| DO compound_statement WHILE L_PAREN logical_expression R_PAREN SEMICOLON
				;

for_statement : FOR L_PAREN initial_expression_list SEMICOLON control_expression_list SEMICOLON increment_expression_list R_PAREN compound_statement
			  ;

initial_expression_list : initial_expression
				  	    |
				        ;

initial_expression : initial_expression COMMA variable_reference ASSIGN_OP logical_expression
				   | initial_expression COMMA logical_expression
				   | logical_expression
				   | variable_reference ASSIGN_OP logical_expression

control_expression_list : control_expression
				  		|
				  		;

control_expression : control_expression COMMA variable_reference ASSIGN_OP logical_expression
				   | control_expression COMMA logical_expression
				   | logical_expression
				   | variable_reference ASSIGN_OP logical_expression
				   ;

increment_expression_list : increment_expression 
						  |
						  ;

increment_expression : increment_expression COMMA variable_reference ASSIGN_OP logical_expression
					 | increment_expression COMMA logical_expression
					 | logical_expression
					 | variable_reference ASSIGN_OP logical_expression
					 ;

function_invoke_statement : ID L_PAREN logical_expression_list R_PAREN SEMICOLON {
    struct symEntry *node = find_ID_Decl(symbolTable,$1);
    printf("%s\n",$1);
    if(node){
        if(node->kind != FUNC_t){
            Not_func_invoke($1);
        }
    }else{
        Func_invoke_not_decl_or_def($1);
    }
}
						  | ID L_PAREN R_PAREN SEMICOLON {
    struct symEntry *node = find_ID_Decl(symbolTable,$1);
    printf("%s\n",$1);
    if(node){
        if(node->kind != FUNC_t){
            Not_func_invoke($1);
        }
    }else{
        Func_invoke_not_decl_or_def($1);
    }
}
						  ;

jump_statement : CONTINUE SEMICOLON
			   | BREAK SEMICOLON
			   | RETURN logical_expression SEMICOLON
			   ;

variable_reference : array_list
				   | ID
				   ;


logical_expression : logical_expression OR_OP logical_term
				   | logical_term
				   ;

logical_term : logical_term AND_OP logical_factor
			 | logical_factor
			 ;

logical_factor : NOT_OP logical_factor
			   | relation_expression
			   ;

relation_expression : arithmetic_expression relation_operator arithmetic_expression
					| arithmetic_expression 
					;

relation_operator : LT_OP
				  | LE_OP
				  | EQ_OP
				  | GE_OP
				  | GT_OP
				  | NE_OP
				  ;

arithmetic_expression : arithmetic_expression ADD_OP term
		   | arithmetic_expression SUB_OP term
                   | relation_expression
		   | term
		   ;

term : term MUL_OP factor
     | term DIV_OP factor
	 | term MOD_OP factor
	 | factor
	 ;

factor : variable_reference
	   | SUB_OP factor
	   | L_PAREN logical_expression R_PAREN
	   | SUB_OP L_PAREN logical_expression R_PAREN
	   | ID L_PAREN logical_expression_list R_PAREN
	   | ID L_PAREN R_PAREN
	   | literal_const
	   | SUB_OP ID L_PAREN logical_expression R_PAREN
	   | SUB_OP ID L_PAREN R_PAREN
	   ;

logical_expression_list : logical_expression_list COMMA logical_expression
						| logical_expression
						;

array_list : ID dimension
		   ;

dimension : dimension ML_BRACE logical_expression MR_BRACE		   
		  | ML_BRACE logical_expression MR_BRACE
		  ;



scalar_type : INT {
    $$ = new_Type(INT_t);
}
			| DOUBLE {
    $$ = new_Type(DOUBLE_t);
}
			| STRING {
    $$ = new_Type(STR_t);
}
			| BOOL {
    $$ = new_Type(BOOLEAN_t);
}
			| FLOAT {
    $$ = new_Type(FLOAT_t);
}
			;
 
literal_const : INT_CONST {
    int tmp = $1;
    $$ = new_ConstAttr(INT_t,(void*)&tmp);
}
			  | SUB_OP INT_CONST {
    int tmp = -$2;
    $$ = new_ConstAttr(INT_t,(void*)&tmp);
}
			  | FLOAT_CONST {
    float tmp = $1;
    $$ = new_ConstAttr(FLOAT_t,(void*)&tmp);
}
			  | SUB_OP FLOAT_CONST {
    float tmp = -$2;
    $$ = new_ConstAttr(FLOAT_t,(void*)&tmp);
}
			  | SCIENTIFIC {
    double tmp = $1;
    $$ = new_ConstAttr(DOUBLE_t,(void*)&tmp);
}
			  | SUB_OP SCIENTIFIC {
    double tmp = -$2;
    $$ = new_ConstAttr(DOUBLE_t,(void*)&tmp);
}
			  | STR_CONST {
    $$ = new_ConstAttr(STR_t,(void*)$1);
}
			  | TRUE {
    int tmp = 1;
    $$ = new_ConstAttr(BOOLEAN_t,(void*)&tmp);
}
			  | FALSE {
    int tmp = 0;
    $$ = new_ConstAttr(BOOLEAN_t,(void*)&tmp);
}
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
	//  fprintf( stderr, "%s\t%d\t%s\t%s\n", "Error found in Line ", linenum, "next token: ", yytext );
}


