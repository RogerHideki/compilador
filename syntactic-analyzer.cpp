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
map <string, vector<ulli>> symbolTable;
map<ulli, vector<string>> scopeStack;
ulli currentScope = 0;


void exitScope();

void verifyVariableUsage();

void declareDouble();

void throwSyntaxError();

void match(const string &expectedToken);

// TODO: REMOVER SE O PROFESSOR APROVAR A CORRECAO DA GRAMATICA
// void argumentos();

void cmd();

void cmds();

void cmdCond();

void condicao();

void dc();

void expressao();

void expIdent();

void fator();

// TODO: REMOVER SE O PROFESSOR APROVAR A CORRECAO DA GRAMATICA
// void listaArg();

void maisCmds();

void maisFatores();

// TODO: REMOVER SE O PROFESSOR APROVAR A CORRECAO DA GRAMATICA
// void maisIdent();

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

void exitScope() {
    for (auto &var: scopeStack[currentScope]) symbolTable[var].pop_back();
    scopeStack.erase(currentScope);
    currentScope--;
}

void verifyVariableUsage() {
    string lexeme = tokens[idx].lexeme;
    if (symbolTable[lexeme].size()) return;
    cout << "Erro Semântico na linha " << tokens[idx].line << ", coluna " << tokens[idx].column
         << ". Variável não declarada: " << lexeme << ".\n";
    exit(1);
}

void declareDouble() {
    string lexeme = tokens[idx].lexeme;
    auto &scopes = symbolTable[lexeme];
    if (scopes.empty() || (scopes.back() != currentScope)) {
        scopes.emplace_back(currentScope);
        scopeStack[currentScope].emplace_back(lexeme);
        return;
    }
    cout << "Erro Semântico na linha " << tokens[idx].line << ", coluna " << tokens[idx].column
         << ". Redeclaração no mesmo escopo: " << lexeme << ".\n";
    exit(1);
}

void throwSyntaxError() {
    if (idx < tokensSize) {
        cout << "Erro de Sintaxe na linha " << tokens[idx].line << ", coluna " << tokens[idx].column
             << ". Símbolo inesperado: " << tokens[idx].lexeme << ".\n";
    } else {
        cout << "Erro de Sintaxe: fim inesperado do arquivo.\n";
    }
    exit(1);
}

void match(const string &expectedToken) {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token != expectedToken) throwSyntaxError();
    idx++;
}

// TODO: REMOVER SE O PROFESSOR APROVAR A CORRECAO DA GRAMATICA
// // ARGUMENTOS -> id <MAIS_IDENT>
// void argumentos() {
//     if (idx >= tokensSize) throwSyntaxError();
//     if (tokens[idx].token == "ID") {
//         match("ID");
//         maisIdent();
//     } else {
//         throwSyntaxError();
//     }
// }

// CMD -> System.out.println (<EXPRESSAO>) | id <RESTO_IDENT>
void cmd() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "PRINTLN") {
        match("PRINTLN");
        match("LEFT_PARENTHESIS");
        expressao();
        match("RIGHT_PARENTHESIS");
        return;
    }
    if (tokens[idx].token == "ID") {
        verifyVariableUsage();
        match("ID");
        restoIdent();
    } else {
        throwSyntaxError();
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
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "IF") {
        match("IF");
        match("LEFT_PARENTHESIS");
        condicao();
        match("RIGHT_PARENTHESIS");
        match("LEFT_CURLY_BRACKET");
        currentScope++;
        cmds();
        match("RIGHT_CURLY_BRACKET");
        exitScope();
        pfalsa();
        return;
    }
    if (tokens[idx].token == "WHILE") {
        match("WHILE");
        match("LEFT_PARENTHESIS");
        condicao();
        match("RIGHT_PARENTHESIS");
        match("LEFT_CURLY_BRACKET");
        currentScope++;
        cmds();
        match("RIGHT_CURLY_BRACKET");
        exitScope();
    } else {
        throwSyntaxError();
    }
}

// CONDICAO -> <EXPRESSAO> <RELACAO> <EXPRESSAO>
void condicao() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "SUBTRACTION_OPERATOR" ||
        tokens[idx].token == "ID" ||
        tokens[idx].token == "REAL_NUMBER" ||
        tokens[idx].token == "LEFT_PARENTHESIS") {
        expressao();
        relacao();
        expressao();
    } else {
        throwSyntaxError();
    }
}

// DC -> <VAR> <MAIS_CMDS>
void dc() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "DOUBLE") {
        var();
        maisCmds();
    } else {
        throwSyntaxError();
    }
}

// EXPRESSAO -> <TERMO> <OUTROS_TERMOS>
void expressao() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "SUBTRACTION_OPERATOR" ||
        tokens[idx].token == "ID" ||
        tokens[idx].token == "REAL_NUMBER" ||
        tokens[idx].token == "LEFT_PARENTHESIS") {
        termo();
        outrosTermos();
    } else {
        throwSyntaxError();
    }
}

// EXP_IDENT -> <EXPRESSAO> | lerDouble()
void expIdent() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "SUBTRACTION_OPERATOR" ||
        tokens[idx].token == "ID" ||
        tokens[idx].token == "REAL_NUMBER" ||
        tokens[idx].token == "LEFT_PARENTHESIS") {
        expressao();
        return;
    }
    if (tokens[idx].token == "LER_DOUBLE") {
        match("LER_DOUBLE");
        match("LEFT_PARENTHESIS");
        match("RIGHT_PARENTHESIS");
    } else {
        throwSyntaxError();
    }
}

// FATOR -> id | numero_real | (<EXPRESSAO>)
void fator() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "ID") {
        verifyVariableUsage();
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
        throwSyntaxError();
    }
}

// TODO: REMOVER SE O PROFESSOR APROVAR A CORRECAO DA GRAMATICA
// // LISTA_ARG -> <ARGUMENTOS> | λ
// void listaArg() {
//     if (idx >= tokensSize) return;
//     if (tokens[idx].token == "ID") {
//         argumentos();
//     }
// }

// MAIS_CMDS -> ;<CMDS>
void maisCmds() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "SEMICOLON") {
        match("SEMICOLON");
        cmds();
    } else {
        throwSyntaxError();
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

// TODO: REMOVER SE O PROFESSOR APROVAR A CORRECAO DA GRAMATICA
// // MAIS_IDENT -> , <ARGUMENTOS> | λ
// void maisIdent() {
//     if (idx >= tokensSize) return;
//     if (tokens[idx].token == "COMMA") {
//         match("COMMA");
//         argumentos();
//     }
// }

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
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "ADDITIVE_OPERATOR") {
        match("ADDITIVE_OPERATOR");
        return;
    }
    if (tokens[idx].token == "SUBTRACTION_OPERATOR") {
        match("SUBTRACTION_OPERATOR");
    } else {
        throwSyntaxError();
    }
}

// OP_MUL -> * | /
void opMul() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "MULTIPLICATION_OPERATOR") {
        match("MULTIPLICATION_OPERATOR");
        return;
    }
    if (tokens[idx].token == "DIVISION_OPERATOR") {
        match("DIVISION_OPERATOR");
    } else {
        throwSyntaxError();
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
        currentScope++;
        cmds();
        match("RIGHT_CURLY_BRACKET");
        exitScope();
    }
}

// PROG -> public class id {  public static void main ( String [ ] id ) {  <CMDS> } }
void prog() {
    if (idx >= tokensSize) throwSyntaxError();
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
        throwSyntaxError();
    }
}

// RELACAO -> == | != | >= | <= | > | <
void relacao() {
    if (idx >= tokensSize) throwSyntaxError();
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
        throwSyntaxError();
    }
}

// RESTO_IDENT -> = <EXP_IDENT> | (<LISTA_ARG>)
void restoIdent() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "ASSIGNMENT_OPERATOR") {
        match("ASSIGNMENT_OPERATOR");
        expIdent();
        return;
    }
        // TODO: REMOVER SE O PROFESSOR APROVAR A CORRECAO DA GRAMATICA
        // if (tokens[idx].token == "LEFT_PARENTHESIS") {
        //     match("LEFT_PARENTHESIS");
        //     listaArg();
        //     match("RIGHT_PARENTHESIS");
        // }
    else {
        throwSyntaxError();
    }
}

// TERMO -> <OP_UN> <FATOR> <MAIS_FATORES>
void termo() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "SUBTRACTION_OPERATOR" ||
        tokens[idx].token == "ID" ||
        tokens[idx].token == "REAL_NUMBER" ||
        tokens[idx].token == "LEFT_PARENTHESIS") {
        opUn();
        fator();
        maisFatores();
    } else {
        throwSyntaxError();
    }
}

// TIPO -> double
void tipo() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "DOUBLE") {
        match("DOUBLE");
    } else {
        throwSyntaxError();
    }
}

// VAR -> <TIPO> <VARS>
void var() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "DOUBLE") {
        tipo();
        vars();
    } else {
        throwSyntaxError();
    }
}

// VARS -> id<MAIS_VAR>
void vars() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "ID") {
        declareDouble();
        match("ID");
        maisVar();
    } else {
        throwSyntaxError();
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
    if (idx < tokensSize) throwSyntaxError();
    return 0;
}
