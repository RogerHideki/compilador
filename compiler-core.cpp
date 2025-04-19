#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <stack>

using namespace std;

#define ulli unsigned long long int

struct Token {
    string token;
    ulli line;
    ulli column;
    string lexeme;
};

struct SymbolInfo {
    ulli scope;
    string memoryIdx;
};

vector<Token> tokens;
ulli tokensSize;
ulli idx = 0;
map<string, vector<SymbolInfo>> symbolTable;
ulli currentMemoryIdx = 0;
map<ulli, vector<string>> scopeStack;
ulli currentScope = 0;

vector<string> objectCode;
stack<string> objectCodeStack;
ofstream objectCodeFile("outputs/object-code.txt");

void exitScope();

void throwSemanticError(const string &errorMessage, ulli tokenIdxOffset);

void verifyVariableUsage();

void declareDouble();

void throwSyntaxError();

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

bool opUn();

void outrosTermos();

void pfalsa(ulli objectCodeIfIdx);

void prog();

void relacao();

void restoIdent();

void termo();

void tipo();

void var();

void vars();

void exitScope() {
    for (auto &var: scopeStack[currentScope]) {
        symbolTable[var].pop_back();
        currentMemoryIdx--;
    }
    scopeStack.erase(currentScope);
    currentScope--;
}

void throwSemanticError(const string &errorMessage, ulli tokenIdxOffset) {
    auto [token, line, column, lexeme] = tokens[idx - tokenIdxOffset];
    cout << "Erro Semântico na linha " << line << ", coluna " << column << errorMessage << lexeme << ".\n";
    exit(1);
}

void verifyVariableUsage() {
    if (symbolTable[tokens[idx].lexeme].size()) return;
    throwSemanticError(". Variável não declarada: ", 0);
}

void declareDouble() {
    string lexeme = tokens[idx].lexeme;
    auto &scopes = symbolTable[lexeme];
    if (scopes.empty() || (scopes.back().scope != currentScope)) {
        scopes.push_back({currentScope, to_string(currentMemoryIdx)});
        currentMemoryIdx++;
        scopeStack[currentScope].emplace_back(lexeme);
        objectCode.emplace_back("ALME 1");
        return;
    }
    throwSemanticError(". Redeclaração no mesmo escopo: ", 0);
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

// ARGUMENTOS -> id <MAIS_IDENT>
void argumentos() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "ID") {
        match("ID");
        maisIdent();
    } else {
        throwSyntaxError();
    }
}

// CMD -> System.out.println (<EXPRESSAO>) | id <RESTO_IDENT>
void cmd() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "PRINTLN") {
        match("PRINTLN");
        match("LEFT_PARENTHESIS");
        expressao();
        match("RIGHT_PARENTHESIS");
        objectCode.emplace_back("IMPR");
        return;
    }
    if (tokens[idx].token == "ID") {
        verifyVariableUsage();
        objectCodeStack.emplace("ARMZ " + symbolTable[tokens[idx].lexeme].back().memoryIdx);
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
        ulli objectCodeIfIdx = objectCode.size();
        objectCode.emplace_back("DSVF ");
        cmds();
        match("RIGHT_CURLY_BRACKET");
        exitScope();
        objectCode[objectCodeIfIdx] += to_string(objectCode.size());
        pfalsa(objectCodeIfIdx);
        return;
    }
    if (tokens[idx].token == "WHILE") {
        match("WHILE");
        match("LEFT_PARENTHESIS");
        ulli objectCodeConditionIdx = objectCode.size();
        condicao();
        match("RIGHT_PARENTHESIS");
        match("LEFT_CURLY_BRACKET");
        currentScope++;
        ulli objectCodeWhileIdx = objectCode.size();
        objectCode.emplace_back("DSVF ");
        cmds();
        match("RIGHT_CURLY_BRACKET");
        exitScope();
        objectCode.emplace_back("DSVI " + to_string(objectCodeConditionIdx));
        objectCode[objectCodeWhileIdx] += to_string(objectCode.size());
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
        objectCode.emplace_back(objectCodeStack.top());
        objectCodeStack.pop();
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
        objectCode.emplace_back("LEIT");
    } else {
        throwSyntaxError();
    }
}

// FATOR -> id | numero_real | (<EXPRESSAO>)
void fator() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "ID") {
        verifyVariableUsage();
        objectCode.emplace_back("CRVL " + symbolTable[tokens[idx].lexeme].back().memoryIdx);
        match("ID");
        return;
    }
    if (tokens[idx].token == "REAL_NUMBER") {
        objectCode.emplace_back("CRCT " + tokens[idx].lexeme);
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

// LISTA_ARG -> <ARGUMENTOS> | λ
void listaArg() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "ID") {
        argumentos();
    }
}

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
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "ADDITIVE_OPERATOR") {
        match("ADDITIVE_OPERATOR");
        objectCodeStack.emplace("SOMA");
        return;
    }
    if (tokens[idx].token == "SUBTRACTION_OPERATOR") {
        match("SUBTRACTION_OPERATOR");
        objectCodeStack.emplace("SUBT");
    } else {
        throwSyntaxError();
    }
}

// OP_MUL -> * | /
void opMul() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "MULTIPLICATION_OPERATOR") {
        match("MULTIPLICATION_OPERATOR");
        objectCodeStack.emplace("MULT");
        return;
    }
    if (tokens[idx].token == "DIVISION_OPERATOR") {
        match("DIVISION_OPERATOR");
        objectCodeStack.emplace("DIVI");
    } else {
        throwSyntaxError();
    }
}

// OP_UN -> - | λ
bool opUn() {
    if (idx >= tokensSize) return false;
    if (tokens[idx].token == "SUBTRACTION_OPERATOR") {
        match("SUBTRACTION_OPERATOR");
        return true;
    }
    return false;
}

// OUTROS_TERMOS -> <OP_AD> <TERMO> <OUTROS_TERMOS> | λ
void outrosTermos() {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "ADDITIVE_OPERATOR" ||
        tokens[idx].token == "SUBTRACTION_OPERATOR") {
        opAd();
        termo();
        objectCode.emplace_back(objectCodeStack.top());
        objectCodeStack.pop();
        outrosTermos();
    }
}

// PFALSA -> else { <CMDS> } | λ
void pfalsa(ulli objectCodeIfIdx) {
    if (idx >= tokensSize) return;
    if (tokens[idx].token == "ELSE") {
        match("ELSE");
        match("LEFT_CURLY_BRACKET");
        currentScope++;
        ulli objectCodeElseIdx = objectCode.size();
        objectCode.emplace_back("DSVI ");
        objectCode[objectCodeIfIdx] = ("DSVF " + to_string(objectCode.size()));
        cmds();
        match("RIGHT_CURLY_BRACKET");
        exitScope();
        objectCode[objectCodeElseIdx] += to_string(objectCode.size());
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
        objectCode.emplace_back("INPP");
        cmds();
        match("RIGHT_CURLY_BRACKET");
        match("RIGHT_CURLY_BRACKET");
        objectCode.emplace_back("PARA");
    } else {
        throwSyntaxError();
    }
}

// RELACAO -> == | != | >= | <= | > | <
void relacao() {
    if (idx >= tokensSize) throwSyntaxError();
    if (tokens[idx].token == "EQUAL") {
        match("EQUAL");
        objectCodeStack.emplace("CPIG");
        return;
    }
    if (tokens[idx].token == "NOT_EQUAL") {
        match("NOT_EQUAL");
        objectCodeStack.emplace("CDES");
        return;
    }
    if (tokens[idx].token == "GREATER_EQUAL") {
        match("GREATER_EQUAL");
        objectCodeStack.emplace("CMAI");
        return;
    }
    if (tokens[idx].token == "LESS_EQUAL") {
        match("LESS_EQUAL");
        objectCodeStack.emplace("CPMI");
        return;
    }
    if (tokens[idx].token == "GREATER") {
        match("GREATER");
        objectCodeStack.emplace("CPMA");
        return;
    }
    if (tokens[idx].token == "LESS") {
        match("LESS");
        objectCodeStack.emplace("CPME");
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
        objectCode.emplace_back(objectCodeStack.top());
        objectCodeStack.pop();
        return;
    }
    if (tokens[idx].token == "LEFT_PARENTHESIS") {
        throwSemanticError(". Método não declarado: ", 1);
        match("LEFT_PARENTHESIS");
        listaArg();
        match("RIGHT_PARENTHESIS");
    } else {
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
        bool mustInvertSign = opUn();
        fator();
        if (mustInvertSign) objectCode.emplace_back("INVE");
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
    for (auto &instruction: objectCode) objectCodeFile << instruction << '\n';
    return 0;
}
