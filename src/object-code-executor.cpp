#include <bits/stdc++.h>

using namespace std;

vector<double> d;
int i = 0;
int s;
int objectCodeSize;

// Reserva m posições na pilha D; m depende do tipo da variável
void alme(int m) {
    d.resize(d.size() + m);
    s += m;
}

// Armazena o topo da pilha no endereço n de D
void armz(int n) {
    d[n] = d[s];
    s--;
    d.pop_back();
}

// Comparação de desigualdade
void cdes() {
    if (d[s - 1] != d[s]) d[s - 1] = 1;
    else d[s - 1] = 0;
    s--;
    d.pop_back();
}

// Comparação maior-igual
void cmai() {
    if (d[s - 1] >= d[s]) d[s - 1] = 1;
    else d[s - 1] = 0;
    s--;
    d.pop_back();
}

// Comparação de igualdade
void cpig() {
    if (d[s - 1] == d[s]) d[s - 1] = 1;
    else d[s - 1] = 0;
    s--;
    d.pop_back();
}

// Comparação de maior
void cpma() {
    if (d[s - 1] > d[s]) d[s - 1] = 1;
    else d[s - 1] = 0;
    s--;
    d.pop_back();
}

// Comparação de menor entre o antecessor e o topo
void cpme() {
    if (d[s - 1] < d[s]) d[s - 1] = 1;
    else d[s - 1] = 0;
    s--;
    d.pop_back();
}

// Comparação menor-igual
void cpmi() {
    if (d[s - 1] <= d[s]) d[s - 1] = 1;
    else d[s - 1] = 0;
    s--;
    d.pop_back();
}

// Carrega constante k no topo da pilha D
void crct(double k) {
    s++;
    d.emplace_back(k);
}

// Carrega valor de endereço n no topo da pilha D
void crvl(int n) {
    s++;
    d.emplace_back(d[n]);
}

// Divide o elemento antecessor pelo elemento do topo
void divi() {
    d[s - 1] /= d[s];
    s--;
    d.pop_back();
}

// Desvio condicional para a instrução de endereço p; o desvio será executado
// caso a condição resultante seja falsa; o valor da condição estará no topo
void dsvf(int p) {
    if (!d[s]) i = p - 1;
    s--;
    d.pop_back();
}

// Desvio incondicional para a instrução de endereço p
void dsvi(int p) {
    i = p - 1;
}

// Imprime valor o valor do topo da pilha na saída
void impr() {
    cout << d[s] << '\n';
    s--;
    d.pop_back();
}

// Inicia programa – será sempre a 1ª instrução
void inpp() {
    s = -1;
}

// Inverte sinal do topo
void inve() {
    d[s] *= -1;
}

// Lê um dado de entrada para o topo da pilha
void leit() {
    s++;
    double input;
    cin >> input;
    d.emplace_back(input);
}

// Multiplica elemento antecessor pelo elemento do topo
void mult() {
    d[s - 1] *= d[s];
    s--;
    d.pop_back();
}

// Termina a execução do programa
void para() {
    i = objectCodeSize;
}

// Soma o elemento antecessor com o topo da pilha; desempilha os dois e empilha o resultado
void soma() {
    d[s - 1] += d[s];
    s--;
    d.pop_back();
}

// Subtrai o antecessor pelo elemento do topo
void subt() {
    d[s - 1] -= d[s];
    s--;
    d.pop_back();
}

int main() {
    ifstream objectCodeFile("outputs/object-code.txt");
    string line;
    vector<pair<int, double>> objectCode;
    map<string, int> m;
    m["ALME"] = 0;
    m["ARMZ"] = 1;
    m["CDES"] = 2;
    m["CMAI"] = 3;
    m["CPIG"] = 4;
    m["CPMA"] = 5;
    m["CPME"] = 6;
    m["CPMI"] = 7;
    m["CRCT"] = 8;
    m["CRVL"] = 9;
    m["DIVI"] = 10;
    m["DSVF"] = 11;
    m["DSVI"] = 12;
    m["IMPR"] = 13;
    m["INPP"] = 14;
    m["INVE"] = 15;
    m["LEIT"] = 16;
    m["MULT"] = 17;
    m["PARA"] = 18;
    m["SOMA"] = 19;
    m["SUBT"] = 20;
    while (getline(objectCodeFile, line)) {
        stringstream ss(line);
        string first, second = "";
        ss >> first >> second;
        objectCode.emplace_back(m[first], second != "" ? stod(second) : 0);
    }
    objectCodeFile.close();
    objectCodeSize = objectCode.size();
    while (i < objectCodeSize) {
        switch (objectCode[i].first) {
            case 0:
                alme(objectCode[i].second);
                break;
            case 1:
                armz(objectCode[i].second);
                break;
            case 2:
                cdes();
                break;
            case 3:
                cmai();
                break;
            case 4:
                cpig();
                break;
            case 5:
                cpma();
                break;
            case 6:
                cpme();
                break;
            case 7:
                cpmi();
                break;
            case 8:
                crct(objectCode[i].second);
                break;
            case 9:
                crvl(objectCode[i].second);
                break;
            case 10:
                divi();
                break;
            case 11:
                dsvf(objectCode[i].second);
                break;
            case 12:
                dsvi(objectCode[i].second);
                break;
            case 13:
                impr();
                break;
            case 14:
                inpp();
                break;
            case 15:
                inve();
                break;
            case 16:
                leit();
                break;
            case 17:
                mult();
                break;
            case 18:
                para();
                break;
            case 19:
                soma();
                break;
            case 20:
                subt();
                break;
        }
        i++;
    }
    return 0;
}