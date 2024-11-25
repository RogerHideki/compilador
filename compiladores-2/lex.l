/*** Definition Section has one variable
which can be accessed inside yylex() 
and main() ***/
%{
#include <stdbool.h>

char output[] = "";
bool hasError = false;
%}

/*** Rule Section has three rules, first rule 
matches with capital letters, second rule
matches with any character except newline and 
third rule does not take input after the enter***/
%%
String\[] |
System.out.println |
class |
double |
else |
if |
main |
public |
return |
static |
void |
while {
  printf("<PALAVRA_RESERVADA, %s>", yytext);
}
[+-] {
	printf("<OP_AD, %s>", yytext);
}
[*/] {
	printf("<OP_MUL, %s>", yytext);
}
== |
!= |
>= |
\<= |
[><] {
	printf("<RELACAO, %s>", yytext);
}
= {
  printf("<OP_ATR>");
}
[}{)(;,] {
	printf("<PONTUACAO, %s>", yytext);
}
[A-Za-z_][0-9A-Za-z_]* {
	printf("<ID, %s>", yytext);
}
[0-9]+(\.[0-9]+)? {
	printf("<NUMERO_REAL, %s>", yytext);
}
[ \n\r\f\t]
. printf("<ERRO>, %s>", yytext);
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
