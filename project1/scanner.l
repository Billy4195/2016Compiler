%{
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
%S C_comm Cpp_comm STRING
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
}
<STRING>\" {
    tokenString("string", str_buf);
    str_buf[0] = '\0';
    BEGIN 0;
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
}
<INITIAL>{whitespace} {
    LIST;
}
<INITIAL>"//" {
    LIST;
    BEGIN Cpp_comm;
}
<INITIAL>"/\*" {
    LIST;
    BEGIN C_comm;
}
<INITIAL>{delimiter} {
    tokenString("delim", yytext); 
}
<INITIAL>#pragma\ statistic\ on {
    LIST;
    Opt_Statistic = 1;
}
<INITIAL>#pragma\ statistic\ off {
    LIST;
    Opt_Statistic = 0;
}
<INITIAL>#pragma\ source\ on {
    LIST;
    Opt_Source = 1;
}
<INITIAL>#pragma\ source\ off {
    Opt_Source = 0;
}
<INITIAL>#pragma\ token\ on {
    LIST;
    Opt_Token = 1;
}
<INITIAL>#pragma\ token\ off {
    LIST;
    Opt_Token = 0;
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
    BEGIN 0;
}
<INITIAL>{operator} {
    sprintf(op,"\"%s\"",yytext);
    token(op);
    op[0] = '\0';
}
<INITIAL>{scientific} {
    tokenString("scientific",yytext);
}
<INITIAL>{integer} {
    tokenString("integer",yytext);
}
<INITIAL>{float} {
    tokenString("float",yytext);
}
. { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

%%

int main( int argc, char **argv )
{
    ID_list.list = malloc( 10 * sizeof(id) );
    ID_list.capacity = 10;
    ID_list.filled = 0;
    if (argc >= 2)
    {
        yyin = fopen( argv[1], "r" );
        if ( NULL == yyin ) {
            fprintf( stderr, "Cannot open: %s\n", argv[1] );
            exit(-1);
        }
    }
    else
    {
        yyin = stdin;
    }

    yylex();

    if (Opt_Statistic)
    {
        int i;
        // Print frequency table
        printf("frequencies of identifiers:\n");
        for(i=0;i<ID_list.filled;i++){
            printf("%s %d\n",ID_list.list[i].name,ID_list.list[i].count);
        }
    }

    exit(0);
}
