#include <bits/stdc++.h>

using namespace std;

#define ulli unsigned long long int

ulli idx = 0;

// ARGUMENTOS -> id <MAIS_IDENT>
void argumentos() {
	if (get<0>tokens[idx] == "ID") {
		match("ID");
		maisIdent();
	} else {
		error();
	}
}

// CMD -> System.out.println (<EXPRESSAO>) | id <RESTO_IDENT>
void cmd() {
	if (get<0>tokens[idx] == "PRINTLN") {
		match("PRINTLN");
		match("LEFT_PARENTHESIS");
		expressao();
		match("RIGHT_PARENTHESIS");
		return;
	}
	if (get<0>tokens[idx] == "ID") {
		match("ID");
		restoIdent();
	} else {
		error();
	}
}

// CMDS -> <CMD><MAIS_CMDS> | <CMD_COND><CMDS> | <DC> | λ
void cmds() {
	if (get<0>tokens[idx] == "PRINTLN" || get<0>tokens[idx] == "ID") {
		cmd();
		maisCmds();
		return;
	}
	if (get<0>tokens[idx] == "IF" || get<0>tokens[idx] == "WHILE") {
		cmdCond();
		cmds();
		return;
	}
	if (get<0>tokens[idx] == "DOUBLE") {
		dc();
	}
}

// CMD_COND -> if (  <CONDICAO> )  {<CMDS>} <PFALSA> | while (  <CONDICAO> )  {<CMDS>}      
void cmdCond() {
	if (get<0>tokens[idx] == "IF") {
		match("IF");
		match("LEFT_PARENTHESIS");
		condicao();
		match("RIGHT_PARENTHESIS");
		match("LEFT_CURLY_BRACKET");
		cmds();
		match("RIGHT_CURLY_BRACKET");
		pfalsa();
		return;
	}
	if (get<0>tokens[idx] == "WHILE") {
		match("WHILE");
		match("LEFT_PARENTHESIS");
		condicao();
		match("RIGHT_PARENTHESIS");
		match("LEFT_CURLY_BRACKET");
		cmds();
		match("RIGHT_CURLY_BRACKET");
	} else {
		error();
	}
}

// CONDICAO -> <EXPRESSAO> <RELACAO> <EXPRESSAO>
void condicao() {
    if (get<0>tokens[idx] == "SUBTRACTION_OPERATOR"
        || get<0>tokens[idx] == "ID"
        || get<0>tokens[idx] == "REAL_NUMBER"
        || get<0>tokens[idx] == "LEFT_PARENTHESIS") {
        expressao();
        relacao();
        expressao();
    } else {
		error();
	}
}

// DC -> <VAR> <MAIS_CMDS>
void dc() {
	if (get<0>tokens[idx] == "DOUBLE") {
		var();
		maisCmds();
	} else {
		error();
	}
}

// EXPRESSAO -> <TERMO> <OUTROS_TERMOS>
void expressao() {
	if (get<0>tokens[idx] == "SUBTRACTION_OPERATOR"
        || get<0>tokens[idx] == "ID"
        || get<0>tokens[idx] == "REAL_NUMBER"
        || get<0>tokens[idx] == "LEFT_PARENTHESIS") {
		termo();
		outrosTermos();
	} else {
		error();
	}
}

// EXP_IDENT -> <EXPRESSAO> | lerDouble()
void expIdent() {
	if (get<0>tokens[idx] == "SUBTRACTION_OPERATOR"
        || get<0>tokens[idx] == "ID"
        || get<0>tokens[idx] == "REAL_NUMBER"
        || get<0>tokens[idx] == "LEFT_PARENTHESIS") {
		expressao();
		return;
	}
	if (get<0>tokens[idx] == "LERDOUBLE") {
		match("LERDOUBLE");
		match("LEFT_PARENTHESIS");
		match("RIGHT_PARENTHESIS");
	} else {
		error();
	}
}

// FATOR -> id | numero_real | (<EXPRESSAO>)
void fator() {
	if (get<0>tokens[idx] == "ID") {
		match("ID");
		return;
	}
	if (get<0>tokens[idx] == "REAL_NUMBER") {
		match("REAL_NUMBER");
		return;
	}
	if (get<0>tokens[idx] == "LEFT_PARENTHESIS") {
		match("LEFT_PARENTHESIS");
		expressao();
		match("RIGHT_PARENTHESIS");
	} else {
		error();
	}
}

// LISTA_ARG -> <ARGUMENTOS> | λ
void listaArg() {
	if (get<0>tokens[idx] == "ID") {
		argumentos();
	}
}

// MAIS_CMDS -> ;<CMDS>
void maisCmds() {
	if (get<0>tokens[idx] == "SEMICOLON") {
		match("SEMICOLON");
		cmds();
	} else {
		error();
	}
}

// MAIS_FATORES -> <OP_MUL> <FATOR> <MAIS_FATORES> | λ
void maisFatores() {
	if (get<0>tokens[idx] == "MULTIPLICATION_OPERATOR"
		|| get<0>tokens[idx] == "DIVISION_OPERATOR") {
		opMul();
		fator();
		maisFatores();
	}
}

// MAIS_IDENT -> , <ARGUMENTOS> | λ
void maisIdent() {
	if (get<0>tokens[idx] == "COMMA") {
		match("COMMA");
		argumentos();
	}
}

// MAIS_VAR -> ,<VARS> | λ
void maisVar() {
	if (get<0>tokens[idx] == "COMMA") {
		match("COMMA");
		vars();
	}
}

// OP_AD -> + | -
void opAd() {
	if (get<0>tokens[idx] == "ADDITIVE_OPERATOR") {
		match("ADDITIVE_OPERATOR");
		return;
	}
	if (get<0>tokens[idx] == "SUBTRACTION_OPERATOR") {
		match("SUBTRACTION_OPERATOR");
	} else {
		error();
	}
}

// OP_MUL -> * | /
void opMul() {
	if (get<0>tokens[idx] == "MULTIPLICATION_OPERATOR") {
		match("MULTIPLICATION_OPERATOR");
		return;
	}
	if (get<0>tokens[idx] == "DIVISION_OPERATOR") {
		match("DIVISION_OPERATOR");
	} else {
		error();
	}
}

// OP_UN -> - | λ
void opUn() {
	if (get<0>tokens[idx] == "SUBTRACTION_OPERATOR") {
		match("SUBTRACTION_OPERATOR");
	}
}

// OUTROS_TERMOS -> <OP_AD> <TERMO> <OUTROS_TERMOS> | λ
void outrosTermos() {
	if (get<0>tokens[idx] == "ADDITIVE_OPERATOR"
		|| get<0>tokens[idx] == "SUBTRACTION_OPERATOR") {
		opAd();
		termo();
		outrosTermos();
	}
}

// PFALSA -> else { <CMDS> } | λ
void pfalsa() {
	if (get<0>tokens[idx] == "ELSE") {
		match("ELSE");
		match("LEFT_CURLY_BRACKET");
		cmds();
		match("RIGHT_CURLY_BRACKET");
	}
}

// PROG -> public class id {  public static void main ( String [ ] id ) {  <CMDS> } }
void prog() {
	if (get<0>tokens[idx] == "PUBLIC") {
		match("PUBLIC");
		match("CLASS");
		match("ID");
		match("LEFT_CURLY_BRACKET");
		match("PUBLIC");
		match("STATIC");
		match("VOID");
		match("MAIN");
		match("LEFT_PARENTHESIS");
		match("STRING[]");
		match("ID");
		match("RIGHT_PARENTHESIS");
		match("LEFT_CURLY_BRACKET");
		cmds();
		match("RIGHT_CURLY_BRACKET");
		match("RIGHT_CURLY_BRACKET");
	} else {
		error();
	}
}

// RELACAO -> == | != | >= | <= | > | <
void relacao() {
	if (get<0>tokens[idx] == "EQUAL") {
		match("EQUAL");
		return;
	}
	if (get<0>tokens[idx] == "NOT_EQUAL") {
		match("NOT_EQUAL");
		return;
	}
	if (get<0>tokens[idx] == "GREATER_EQUAL") {
		match("GREATER_EQUAL");
		return;
	}
	if (get<0>tokens[idx] == "LESS_EQUAL") {
		match("LESS_EQUAL");
		return;
	}
	if (get<0>tokens[idx] == "GREATER") {
		match("GREATER");
		return;
	}
	if (get<0>tokens[idx] == "LESS") {
		match("LESS");
	} else {
		error();
	}
}

// RESTO_IDENT -> = <EXP_IDENT> | (<LISTA_ARG>)
void restoIdent() {
	if (get<0>tokens[idx] == "ASSIGNMENT_OPERATOR") {
		match("ASSIGNMENT_OPERATOR");
		expIdent();
		return;
	}
	if (get<0>tokens[idx] == "LEFT_PARENTHESIS") {
		match("LEFT_PARENTHESIS");
		listaArg();
		match("RIGHT_PARENTHESIS");
	} else {
		error();
	}
}

// TERMO -> <OP_UN> <FATOR> <MAIS_FATORES>
void termo() {
	if (get<0>tokens[idx] == "SUBTRACTION_OPERATOR"
		|| get<0>tokens[idx] == "ID"
		|| get<0>tokens[idx] == "REAL_NUMBER"
		|| get<0>tokens[idx] == "LEFT_PARENTHESIS") {
		opUn();
		fator();
		maisFatores();
	} else {
		error();
	}
}

// TIPO -> double
void tipo() {
	if (get<0>tokens[idx] == "DOUBLE") {
		match("DOUBLE");
	} else {
		error();
	}
}

// VAR -> <TIPO> <VARS>
void var() {
	if (get<0>tokens[idx] == "DOUBLE") {
		tipo();
		vars();
	} else {
		error();
	}
}

// VARS -> id<MAIS_VAR>
void vars() {
	if (get<0>tokens[idx] == "ID") {
		match("ID");
		maisVar();
	} else {
		error();
	}
}

void error() {
	cout << "Erro de Sintaxe\n";
	exit(1);
}

void match(string expectedToken) {
	if (get<0>tokens[idx++] == expectedToken) return;
	error();
}

int main () {
	ifstream inputFile("lex-output.txt");
    string inputFileLine;
	vector<tuple<string, ulli, ulli, string>> tokens;
	while (getline(inputFile, inputFileLine)) {
		stringstream ss(inputFileLine);
		string token, line, column, lexeme = "";
		ss >> token >> line >> column >> lexeme;
		if (token == "ERROR") {
			cout << "Erro léxico na linha " << line << ", coluna " << column << '\n';
			return 0; // TODO: VERIFICAR ESSA LINHA
		}
		tokens.emplace_back(token, stoull(line), stoull(column), lexeme);
	}
	prog();
	return 0;
}
