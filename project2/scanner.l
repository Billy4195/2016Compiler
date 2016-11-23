%{
#include "parser.tab.h"
#define LIST                { strncat(buf, yytext, sizeof(buf) - strlen(buf) - 1); }
#define token(t)            { LIST; if (Opt_Token) printf("<%s>\n", t); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define MAX_LINE_LENGTH 257
#define MAX_ID_LENGTH 257
#define LIST_STR(str)            { strncat(str_buf, str, sizeof(str_buf) - strlen(str_buf) -1); }
typedef struct{
    char name[257];
    unsigned int count;
}id;
typedef struct{
    id *list;
    unsigned int filled;
    unsigned int capacity;
}id_list;
int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
char buf[MAX_LINE_LENGTH];
char str_buf[MAX_LINE_LENGTH];
char op[5];
id_list ID_list;
%}
%S C_comm Cpp_comm STRING PRAGMA
keyword while|do|if|else|true|false|for|int|print|const|read|boolean|bool|void|float|double|string|continue|break|return
whitespace [ \t]
delimiter ,|;|\(|\)|\[|\]|\{|\}
letter [a-zA-Z]+
digit [0-9]
operator \+|-|\*|\/|%|=|<|<=|!=|>=|>|==|&&|\|\||!
integer 0|[1-9][0-9]*
float [0-9]*\.[0-9]*
scientific [0-9]*\.?[0-9]*(E|e)(\+|-)?[0-9]*
%%
<C_comm>"*/" {
    LIST;
    BEGIN 0;
}
<Cpp_comm>. |
<C_comm>.   {
    LIST;
}
<INITIAL>{keyword} {
    tokenString("KW", yytext);
    if( strcmp(yytext, "while") == 0){
        return WHILE;
    }else if( strcmp(yytext, "do") == 0){
        return DO;
    }else if( strcmp(yytext, "if") == 0){
        return IF;
    }else if( strcmp(yytext, "else") == 0){
        return ELSE;
    }else if( strcmp(yytext, "true") == 0){
        return TRUE;
    }else if( strcmp(yytext, "false") == 0){
        return FALSE;
    }else if( strcmp(yytext, "for") == 0){
        return FOR;
    }else if( strcmp(yytext, "int") == 0){
        return INT;
    }else if( strcmp(yytext, "print") == 0){
        return PRINT;
    }else if( strcmp(yytext, "const") == 0){
        return CONST;
    }else if( strcmp(yytext, "read") == 0){
        return READ;
    }else if( strcmp(yytext, "boolean") == 0){
        return BOOLEAN;
    }else if( strcmp(yytext, "bool") == 0){
        return BOOL;
    }else if( strcmp(yytext, "void") == 0){
        return VOID;
    }else if( strcmp(yytext, "float") == 0){
        return FLOAT;
    }else if( strcmp(yytext, "double") == 0){
        return DOUBLE;
    }else if( strcmp(yytext, "string") == 0){
        return STRING;
    }else if( strcmp(yytext, "continue") == 0){
        return CONTINUE;
    }else if( strcmp(yytext, "break") == 0){
        return BREAK;
    }else if( strcmp(yytext, "return") == 0){
        return RETURN;
    }
}
<STRING>\" {
    tokenString("string", str_buf);
    str_buf[0] = '\0';
    BEGIN 0;
    return str;
}
<INITIAL>\" {
    LIST;
    BEGIN STRING;
}
<STRING>[\\][\\] {
    LIST_STR(yytext+1);
    LIST;
}
<STRING>[\\][\"] {
    LIST_STR(yytext+1);
    LIST;
}
<STRING>[^"\\] {
    LIST_STR(yytext);
    LIST;
}
<INITIAL>{letter}({letter}|{digit})* {
    tokenString("id", yytext);
    int i;
    char id_exist=0;
    for(i=0;i<ID_list.filled;i++){
        if(!strcmp(ID_list.list[i].name,yytext)){
            ID_list.list[i].count++;
            id_exist = 1;
        }
    }
    if(!id_exist){
        if(ID_list.filled == ID_list.capacity){
            id *tmp=malloc( (ID_list.capacity+10)*sizeof(id) );
            memset(tmp, 0, (ID_list.capacity+10)*sizeof(id) );
            memcpy(tmp, ID_list.list, ID_list.capacity*sizeof(id) );
            ID_list.capacity += 10;
            free(ID_list.list);
            ID_list.list = tmp;
        }         
        strcpy(ID_list.list[ID_list.filled].name,yytext);
        ID_list.list[ID_list.filled].count = 1;
        ID_list.filled++;
    }
    return ID;
}
<PRAGMA>{whitespace} |
<INITIAL>{whitespace} {
    LIST;
}
<PRAGMA>"//" |
<INITIAL>"//" {
    LIST;
    BEGIN Cpp_comm;
}
<PRAGMA>"/\*" |
<INITIAL>"/\*" {
    LIST;
    BEGIN C_comm;
}
<INITIAL>{delimiter} {
    tokenString("delim", yytext); 
    switch(*yytext){
        case ',':
            return COMMA;
        case ';':
            return SEMICOLON;
        case '(':
            return L_PAREN;
        case ')':
            return R_PAREN;
        case '[':
            return L_BRACKET;
        case ']':
            return R_BRACKET;
        case '{':
            return L_BRACE;
        case '}':
            return R_BRACE;
    }
}
<INITIAL>#pragma\ statistic\ on {
    LIST;
    Opt_Statistic = 1;
    BEGIN PRAGMA;
}
<INITIAL>#pragma\ statistic\ off {
    LIST;
    Opt_Statistic = 0;
    BEGIN PRAGMA;
}
<INITIAL>#pragma\ source\ on {
    LIST;
    Opt_Source = 1;
    BEGIN PRAGMA;
}
<INITIAL>#pragma\ source\ off {
    Opt_Source = 0;
    BEGIN PRAGMA;
}
<INITIAL>#pragma\ token\ on {
    LIST;
    Opt_Token = 1;
    BEGIN PRAGMA;
}
<INITIAL>#pragma\ token\ off {
    LIST;
    Opt_Token = 0;
    BEGIN PRAGMA;
}
<C_comm>\n {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}
\n {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
    BEGIN INITIAL;
}
<INITIAL>{operator} {
    sprintf(op,"\"%s\"",yytext);
    token(op);
    op[0] = '\0';
    if( strcmp(yytext, "+") == 0){
        return PLUS;
    }else if( strcmp(yytext, "-") == 0){
        return SUB;
    }else if( strcmp(yytext, "*") == 0){
        return MUL;
    }else if( strcmp(yytext, "/") == 0){
        return DIV;
    }else if( strcmp(yytext, "%") == 0){
        return MOD;
    }else if( strcmp(yytext, "=") == 0){
        return ASSIGN;
    }else if( strcmp(yytext, "<") == 0){
        return LT;
    }else if( strcmp(yytext, "<=") == 0){
        return LE;
    }else if( strcmp(yytext, "!=") == 0){
        return NE;
    }else if( strcmp(yytext, ">=") == 0){
        return GE;
    }else if( strcmp(yytext, ">") == 0){
        return GT;
    }else if( strcmp(yytext, "==") == 0){
        return EQ;
    }else if( strcmp(yytext, "&&") == 0){
        return AND;
    }else if( strcmp(yytext, "||") == 0){
        return OR;
    }else if( strcmp(yytext, "!") == 0){
        return NOT;
    }
}
<INITIAL>{scientific} {
    tokenString("scientific",yytext);
    return scien;
}
<INITIAL>{integer} {
    tokenString("integer",yytext);
    return int_;
}
<INITIAL>{float} {
    tokenString("float",yytext);
    return float_;
}
. { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

%%
