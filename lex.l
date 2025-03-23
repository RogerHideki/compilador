%{
%}

%%
String\[] {
  printf("<STRING[]>\n");
}
System.out.println {
  printf("<PRINTLN>\n");
}
class {
  printf("<CLASS>\n");
}
double {
  printf("<DOUBLE>\n");
}
else {
  printf("<ELSE>\n");
}
if {
  printf("<IF>\n");
}
main {
  printf("<MAIN>\n");
}
public {
  printf("<PUBLIC>\n");
}
static {
  printf("<STATIC>\n");
}
void {
  printf("<VOID>\n");
}
while {
  printf("<WHILE>\n");
}
\+ {
  printf("<ADDITIVE_OPERATOR>\n");
}
- {
  printf("<SUBTRACTION_OPERATOR>\n");
}
\* {
  printf("<MULTIPLICATION_OPERATOR>\n");
}
\/ {
  printf("<DIVISION_OPERATOR>\n");
}
== {
  printf("<EQUAL>\n");
}
!= {
  printf("<NOT_EQUAL>\n");
}
>= {
  printf("<GREATER_EQUAL>\n");
}
\<= {
  printf("<LESS_EQUAL>\n");
}
> {
  printf("<GREATER>\n");
}
\< {
  printf("<LESS>\n");
}
= {
  printf("<ASSIGNMENT_OPERATOR>\n");
}
\{ {
	printf("<LEFT_CURLY_BRACKET>\n");
}
\} {
	printf("<RIGHT_CURLY_BRACKET>\n");
}
\( {
	printf("<LEFT_PARENTHESIS>\n");
}
\) {
	printf("<RIGHT_PARENTHESIS>\n");
}
; {
	printf("<SEMICOLON>\n");
}
, {
	printf("<COMMA>\n");
}
lerDouble {
	printf("<LER_DOUBLE>\n");
}
[A-Za-z_][0-9A-Za-z_]* {
	printf("<ID, %s>\n", yytext);
}
[0-9]+(\.[0-9]+)? {
	printf("<REAL_NUMBER, %s>\n", yytext);
}
[ \n\r\f\t]+ {}
[^ \n\r\f\t]+ {
  printf("<ERRO>, %s>\n", yytext);
}
%%

int yywrap(){}
int main(){
  yylex();
  return 0;
}
