#include <bits/stc++.h>

using namespace std;

#define ulli unsigned long long int

// CMDS -> <CMD><MAIS_CMDS> | <CMD_COND><CMDS> | <DC> | λ
bool cmds() {

}

// PROG -> public class id {  public static void main ( String [ ] id ) {  <CMDS> } } 
void prog() {

}

int main () {
	ifstream inputFile("lex-output.txt");
    string inputFileLine;
	vector<tuple<string, ulli, ulli, string>> tokens;
	while (getline(inputFile, inputFileLine)) {
		stringstream ss(inputFileLine);
		string token, line, column, lexeme = "";
		ss >> token >> line >> column >> lexeme;
		tokens.emplace_back(token, stoull(line), stoull(column), lexeme,);
	}
	return 0;
}
