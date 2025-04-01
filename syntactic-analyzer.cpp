#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

#define ulli unsigned long long int

struct Token {
    string token;
    ulli line;
    ulli column;
    string lexeme;
};

vector<Token> tokens;
ulli tokensSize;
ulli idx = 0;

void error();

void match(const string &expectedToken);

void argumentos();

void cmd();

void cmds();

void cmdCond();

void condicao();

void dc();

void expressao();

void expIdent();

void fator();

void listaArg();

void maisCmds();

void maisFatores();

void maisIdent();

void maisVar();

void opAd();

void opMul();

void opUn();

void outrosTermos();

void pfalsa();

void prog();

void relacao();

void restoIdent();

void termo();

void tipo();

void var();

void vars();

void error() {
    if (idx < tokensSize) {
        cout << "Erro de Sintaxe na linha " << tokens[idx].line << ", coluna " << tokens[idx].column
             << ". Símbolo inesperado: " << tokens[idx].lexeme << ".\n";
    } else {
        cout << "Erro de Sintaxe: fim inesperado do arquivo.\n";
    }
    exit(1);
}

void match(const string &expectedToken) {
    if (idx >= tokensSize) error();
    if (tokens[idx].token != expectedToken) error();
    idx++;
}

// ARGUMENTOS -> id <MAIS_IDENT>
void argumentos() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "ID") {
        match("ID");
        maisIdent();
    } else {
        error();
    }
}

// CMD -> System.out.println (<EXPRESSAO>) | id <RESTO_IDENT>
void cmd() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "PRINTLN") {
        match("PRINTLN");
        match("LEFT_PARENTHESIS");
        expressao();
        match("RIGHT_PARENTHESIS");
        return;
    }
    if (tokens[idx].token == "ID") {
        match("ID");
        restoIdent();
    } else {
        error();
    }
}

// CMDS -> <CMD><MAIS_CMDS> | <CMD_COND><CMDS> | <DC> | λ
void cmds() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "PRINTLN" ||
        tokens[idx].token == "ID") {
        cmd();
        maisCmds();
        return;
    }
    if (tokens[idx].token == "IF" ||
        tokens[idx].token == "WHILE") {
        cmdCond();
        cmds();
        return;
    }
    if (tokens[idx].token == "DOUBLE") {
        dc();
    }
}

// CMD_COND -> if (  <CONDICAO> )  {<CMDS>} <PFALSA> | while (  <CONDICAO> )  {<CMDS>}
void cmdCond() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "IF") {
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
    if (tokens[idx].token == "WHILE") {
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
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "SUBTRACTION_OPERATOR" ||
        tokens[idx].token == "ID" ||
        tokens[idx].token == "REAL_NUMBER" ||
        tokens[idx].token == "LEFT_PARENTHESIS") {
        expressao();
        relacao();
        expressao();
    } else {
        error();
    }
}

// DC -> <VAR> <MAIS_CMDS>
void dc() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "DOUBLE") {
        var();
        maisCmds();
    } else {
        error();
    }
}

// EXPRESSAO -> <TERMO> <OUTROS_TERMOS>
void expressao() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "SUBTRACTION_OPERATOR" ||
        tokens[idx].token == "ID" ||
        tokens[idx].token == "REAL_NUMBER" ||
        tokens[idx].token == "LEFT_PARENTHESIS") {
        termo();
        outrosTermos();
    } else {
        error();
    }
}

// EXP_IDENT -> <EXPRESSAO> | lerDouble()
void expIdent() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "SUBTRACTION_OPERATOR" ||
        tokens[idx].token == "ID" ||
        tokens[idx].token == "REAL_NUMBER" ||
        tokens[idx].token == "LEFT_PARENTHESIS") {
        expressao();
        return;
    }
    if (tokens[idx].token == "LERDOUBLE") {
        match("LERDOUBLE");
        match("LEFT_PARENTHESIS");
        match("RIGHT_PARENTHESIS");
    } else {
        error();
    }
}

// FATOR -> id | numero_real | (<EXPRESSAO>)
void fator() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "ID") {
        match("ID");
        return;
    }
    if (tokens[idx].token == "REAL_NUMBER") {
        match("REAL_NUMBER");
        return;
    }
    if (tokens[idx].token == "LEFT_PARENTHESIS") {
        match("LEFT_PARENTHESIS");
        expressao();
        match("RIGHT_PARENTHESIS");
    } else {
        error();
    }
}

// LISTA_ARG -> <ARGUMENTOS> | λ
void listaArg() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "ID") {
        argumentos();
    }
}

// MAIS_CMDS -> ;<CMDS>
void maisCmds() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "SEMICOLON") {
        match("SEMICOLON");
        cmds();
    } else {
        error();
    }
}

// MAIS_FATORES -> <OP_MUL> <FATOR> <MAIS_FATORES> | λ
void maisFatores() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "MULTIPLICATION_OPERATOR" ||
        tokens[idx].token == "DIVISION_OPERATOR") {
        opMul();
        fator();
        maisFatores();
    }
}

// MAIS_IDENT -> , <ARGUMENTOS> | λ
void maisIdent() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "COMMA") {
        match("COMMA");
        argumentos();
    }
}

// MAIS_VAR -> ,<VARS> | λ
void maisVar() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "COMMA") {
        match("COMMA");
        vars();
    }
}

// OP_AD -> + | -
void opAd() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "ADDITIVE_OPERATOR") {
        match("ADDITIVE_OPERATOR");
        return;
    }
    if (tokens[idx].token == "SUBTRACTION_OPERATOR") {
        match("SUBTRACTION_OPERATOR");
    } else {
        error();
    }
}

// OP_MUL -> * | /
void opMul() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "MULTIPLICATION_OPERATOR") {
        match("MULTIPLICATION_OPERATOR");
        return;
    }
    if (tokens[idx].token == "DIVISION_OPERATOR") {
        match("DIVISION_OPERATOR");
    } else {
        error();
    }
}

// OP_UN -> - | λ
void opUn() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "SUBTRACTION_OPERATOR") {
        match("SUBTRACTION_OPERATOR");
    }
}

// OUTROS_TERMOS -> <OP_AD> <TERMO> <OUTROS_TERMOS> | λ
void outrosTermos() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "ADDITIVE_OPERATOR" ||
        tokens[idx].token == "SUBTRACTION_OPERATOR") {
        opAd();
        termo();
        outrosTermos();
    }
}

// PFALSA -> else { <CMDS> } | λ
void pfalsa() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "ELSE") {
        match("ELSE");
        match("LEFT_CURLY_BRACKET");
        cmds();
        match("RIGHT_CURLY_BRACKET");
    }
}

// PROG -> public class id {  public static void main ( String [ ] id ) {  <CMDS> } }
void prog() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "PUBLIC") {
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
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "EQUAL") {
        match("EQUAL");
        return;
    }
    if (tokens[idx].token == "NOT_EQUAL") {
        match("NOT_EQUAL");
        return;
    }
    if (tokens[idx].token == "GREATER_EQUAL") {
        match("GREATER_EQUAL");
        return;
    }
    if (tokens[idx].token == "LESS_EQUAL") {
        match("LESS_EQUAL");
        return;
    }
    if (tokens[idx].token == "GREATER") {
        match("GREATER");
        return;
    }
    if (tokens[idx].token == "LESS") {
        match("LESS");
    } else {
        error();
    }
}

// RESTO_IDENT -> = <EXP_IDENT> | (<LISTA_ARG>)
void restoIdent() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "ASSIGNMENT_OPERATOR") {
        match("ASSIGNMENT_OPERATOR");
        expIdent();
        return;
    }
    if (tokens[idx].token == "LEFT_PARENTHESIS") {
        match("LEFT_PARENTHESIS");
        listaArg();
        match("RIGHT_PARENTHESIS");
    } else {
        error();
    }
}

// TERMO -> <OP_UN> <FATOR> <MAIS_FATORES>
void termo() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "SUBTRACTION_OPERATOR" ||
        tokens[idx].token == "ID" ||
        tokens[idx].token == "REAL_NUMBER" ||
        tokens[idx].token == "LEFT_PARENTHESIS") {
        opUn();
        fator();
        maisFatores();
    } else {
        error();
    }
}

// TIPO -> double
void tipo() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "DOUBLE") {
        match("DOUBLE");
    } else {
        error();
    }
}

// VAR -> <TIPO> <VARS>
void var() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "DOUBLE") {
        tipo();
        vars();
    } else {
        error();
    }
}

// VARS -> id<MAIS_VAR>
void vars() {
    if (idx >= tokensSize) error();
    if (tokens[idx].token == "ID") {
        match("ID");
        maisVar();
    } else {
        error();
    }
}

int main() {
    ifstream inputFile("lex-output.txt");
    string inputFileLine;
    while (getline(inputFile, inputFileLine)) {
        stringstream ss(inputFileLine);
        string token, line, column, lexeme = "";
        ss >> token >> line >> column >> lexeme;
        if (token == "ERROR") {
            cout << "Erro léxico na linha " << line << ", coluna " << column << ".\n";
            return 0;
        }
        tokens.push_back({token, stoull(line), stoull(column), lexeme});
    }
    tokensSize = tokens.size();
    prog();
    if (idx < tokensSize) error();
    return 0;
}
