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
String\[] { printToken("STRING[]", "String[]"); }
System.out.println { printToken("PRINTLN", "System.out.println"); }
class { printToken("CLASS", "class"); }
double { printToken("DOUBLE", "double"); }
else { printToken("ELSE", "else"); }
if { printToken("IF", "if"); }
lerDouble { printToken("LER_DOUBLE", "lerDouble"); }
main { printToken("MAIN", "main"); }
public { printToken("PUBLIC", "public"); }
static { printToken("STATIC", "static"); }
void { printToken("VOID", "void"); }
while { printToken("WHILE", "while"); }

\+ { printToken("ADDITIVE_OPERATOR", "+"); }
- { printToken("SUBTRACTION_OPERATOR", "-"); }
\* { printToken("MULTIPLICATION_OPERATOR", "*"); }
\/ { printToken("DIVISION_OPERATOR", "/"); }

== { printToken("EQUAL", "=="); }
!= { printToken("NOT_EQUAL", "!="); }
>= { printToken("GREATER_EQUAL", ">="); }
\<= { printToken("LESS_EQUAL", "<="); }
> { printToken("GREATER", ">"); }
\< { printToken("LESS", "<"); }

= { printToken("ASSIGNMENT_OPERATOR", "="); }

\{ { printToken("LEFT_CURLY_BRACKET", "{"); }
\} { printToken("RIGHT_CURLY_BRACKET", "}"); }
\( { printToken("LEFT_PARENTHESIS", "("); }
\) { printToken("RIGHT_PARENTHESIS", ")"); }

; { printToken("SEMICOLON", ";"); }
, { printToken("COMMA", ","); }

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