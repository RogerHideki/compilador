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
  printf("<STRING[]>");
}
System.out.println {
  printf("<PRINTLN>");
}
class {
  printf("<CLASS>");
}
double {
  printf("<DOUBLE>");
}
else {
  printf("<ELSE>");
}
if {
  printf("<IF>");
}
main {
  printf("<MAIN>");
}
public {
  printf("<PUBLIC>");
}
static {
  printf("<STATIC>");
}
void {
  printf("<VOID>");
}
while {
  printf("<WHILE>");
}
+ {
  printf("<ADDITIVE_OPERATOR>");
}
- {
  printf("<SUBTRACTION_OPERATOR>");
}
* {
  printf("<MULTIPLICATION_OPERATOR>");
}
/ {
  printf("<DIVISION_OPERATOR>");
}
== {
  printf("<EQUAL>");
}
!= {
  printf("<NOT_EQUAL>");
}
>= {
  printf("<GREATER_EQUAL>");
}
\<= {
  printf("<LESS_EQUAL>");
}
> {
  printf("<GREATER>");
}
< {
  printf("<LESS>");
}
= {
  printf("<ASSIGNMENT_OPERATOR>");
}
{ {
	printf("<LEFT_CURLY_BRACKET>");
}
} {
	printf("<RIGHT_CURLY_BRACKET>");
}
( {
	printf("<LEFT_PARENTHESIS>");
}
) {
	printf("<RIGHT_PARENTHESIS>");
}
; {
	printf("<SEMICOLON>");
}
, {
	printf("<COMMA>");
}
lerDouble {
	printf("<LER_DOUBLE>");
}
[A-Za-z_][0-9A-Za-z_]* {
	printf("<ID, %s>", yytext);
}
[0-9]+(\.[0-9]+)? {
	printf("<REAL_NUMBER, %s>", yytext);
}
[ \n\r\f\t]
. {
	printf("<ERRO>, %s>", yytext);
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
