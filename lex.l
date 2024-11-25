/*** Definition Section has one variable
which can be accessed inside yylex() 
and main() ***/
%{
int stringSize;
char substring[3];
%}

/*** Rule Section has three rules, first rule 
matches with capital letters, second rule
matches with any character except newline and 
third rule does not take input after the enter***/
%%
A |
ALGORITMO |
ATRIBUIR |
DECLARACOES |
ENQUANTO |
ENTAO |
FIM |
IMPRIMIR |
INICIO |
INTEIRO |
LER |
REAL |
SE {
	stringSize = sizeof(yytext);
	if (stringSize < 3) strncpy(substring, yytext, stringSize);
	else strncpy(substring, yytext, 3);
  printf("< %s >", substring);
}
[-+*/] {
	printf("< OpArit,'%s' >", yytext);
}
\< |
\<= |
\>= |
\> |
= |
\<\> {
	printf("< OpRel,'%s' >", yytext);
}
E |
OU {
	printf("< OpBool,'%s' >", yytext);
}
: {
	printf("< Delim >");
}
\( {
	printf("< AP >");
}
\) {
	printf("< FP >");
}
[A-Za-z]{1}[0-9A-Za-z]* {
	printf("< Var,'%s' >", yytext);
}
0 |
[1-9]{1}[0-9]* {
	printf("< NumI,'%s' >", yytext);
}
(0|([1-9]{1}[0-9]*))\.[0-9]+ {
	printf("< NumR,'%s' >", yytext);
}
'[A-Za-z]*' {
	printf("< Str,'%s' >", yytext);
}
<<EOF>> {
	printf("< EOF >");
	return 0;
}
%.*\n |
. |
\n
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
