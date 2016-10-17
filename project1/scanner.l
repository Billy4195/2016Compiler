%{
  int line=0;
%}
whitespace [ \t\n]*
%%
{whitespace} ;
.* {
  printf("%d:",++line);
  ECHO;
  printf("\n");
}
