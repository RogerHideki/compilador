%{
#include <stdio.h>

unsigned long long int line, column;

void printToken(const char *token, const char *lexeme) {
  printf("%s %llu %llu", token, line, column);
  if (lexeme) printf(" %s", lexeme);
  printf("\n");
  column += yyleng;
}
%}

%%
String\[] { printToken("STRING[]", NULL); }
System.out.println { printToken("PRINTLN", NULL); }
class { printToken("CLASS", NULL); }
double { printToken("DOUBLE", NULL); }
else { printToken("ELSE", NULL); }
if { printToken("IF", NULL); }
lerDouble { printToken("LER_DOUBLE", NULL); }
main { printToken("MAIN", NULL); }
public { printToken("PUBLIC", NULL); }
static { printToken("STATIC", NULL); }
void { printToken("VOID", NULL); }
while { printToken("WHILE", NULL); }

\+ { printToken("ADDITIVE_OPERATOR", NULL); }
- { printToken("SUBTRACTION_OPERATOR", NULL); }
\* { printToken("MULTIPLICATION_OPERATOR", NULL); }
\/ { printToken("DIVISION_OPERATOR", NULL); }

== { printToken("EQUAL", NULL); }
!= { printToken("NOT_EQUAL", NULL); }
>= { printToken("GREATER_EQUAL", NULL); }
\<= { printToken("LESS_EQUAL", NULL); }
> { printToken("GREATER", NULL); }
\< { printToken("LESS", NULL); }

= { printToken("ASSIGNMENT_OPERATOR", NULL); }

\{ { printToken("LEFT_CURLY_BRACKET", NULL); }
\} { printToken("RIGHT_CURLY_BRACKET", NULL); }
\( { printToken("LEFT_PARENTHESIS", NULL); }
\) { printToken("RIGHT_PARENTHESIS", NULL); }

; { printToken("SEMICOLON", NULL); }
, { printToken("COMMA", NULL); }

[A-Za-z_][0-9A-Za-z_]* { printToken("ID", yytext); }
[0-9]+(\.[0-9]+)? { printToken("REAL_NUMBER", yytext); }

[ \t]+ { column += yyleng; }
\r\n|\n {
  line++;
  column = 1;
}

. { printToken("ERROR", NULL); }
%%

int yywrap() { return 1; }

int main() {
  line = column = 1;
  yylex();
  return 0;
}