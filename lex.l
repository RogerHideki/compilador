/*** Definition Section has one variable
which can be accessed inside yylex() 
and main() ***/
%{
%}

/*** Rule Section has three rules, first rule 
matches with capital letters, second rule
matches with any character except newline and 
third rule does not take input after the enter***/
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
[ \n\r\f\t]
. {
	printf("<ERRO>, %s>\n", yytext);
}
%%
/*** Code Section prints the number of
capital letter present in the given input***/
int yywrap(){}
int main(){

// Explanation:
// yywrap() - wraps the above rule section
/* yyin - takes the file pointer 
		which contains the input*/
/* yylex() - this is the main flex function
		which runs the Rule Section*/
// yytext is the text in the buffer

// Uncomment the lines below 
// to take input from file
// FILE *fp;
// char filename[50];
// printf("Enter the filename: \n");
// scanf("%s",filename);
// fp = fopen(filename,"r");
// yyin = fp;

yylex();
return 0;
}
