#include "tokens.hpp"
#include <iostream>
#include <string>
#include <stack>

using namespace std;

#define LPAREN 23
#define	RPAREN 24
#define	LBRACE 25
#define	RBRACE 26
#define INVALID_CHAR -1
#define TAB "\t"

int tab = 0;
stack<int> brackets;

string tokenNumberToString(int token_num){
	string token_string;
	switch (token_num)
	{
	case 1:
		token_string = "VOID";
		break;
	case 2:
		token_string = "INT";
		break;
	case 3:
		token_string = "BYTE";
		break;
	case 4:
		token_string = "B";
		break;
	case 5:
		token_string = "BOOL";
		break;
	case 6:
		token_string = "AND";
		break;
	case 7:
		token_string = "OR";
		break;
	case 8:
		token_string = "NOT";
		break;
	case 9:
		token_string = "TRUE";
		break;
	case 10:
		token_string = "FALSE";
		break;
	case 11:
		token_string = "RETURN";
		break;
	case 12:
		token_string = "IF";
		break;
	case 13:
		token_string = "ELSE";
		break;
	case 14:
		token_string = "WHILE";
		break;
	case 15:
		token_string = "BREAK";
		break;
	case 16:
		token_string = "CONTINUE";
		break;
	case 17:
		token_string = "SWITCH";
		break;
	case 18:
		token_string = "CASE";
		break;
	case 19:
		token_string = "DEFAULT";
		break;
	case 20:
		token_string = "COLON";
		break;
	case 21:
		token_string = "SC";
		break;
	case 22:
		token_string = "COMMA";
		break;
	case 27:
		token_string = "ASSIGN";
		break;
	case 28:
		token_string = "RELOP";
		break;
	case 29:
		token_string = "BINOP";
		break;
	case 30:
		token_string = "COMMENT";
		break;
	case 31:
		token_string = "ID";
		break;
	case 32:
		token_string = "NUM";
		break;
	case 33:
		token_string = "STRING";
		break;
	default:
		break;
	}
	return token_string;
}

void errorInvalidChar(char* x){
	cout << "Error " << x << endl;
	exit(0);
}

void errorInvalidToken(int n){
	string token = tokenNumberToString(n);
	cout << "Error: " << token << endl;
	exit(0);
}

void errorBadExpression(){
	cout << "Error: Bad Expression" << endl;
	exit(0);
}

int main()
{
	int token;
	while((token = yylex())) {
		if (token == INVALID_CHAR) {
			errorInvalidChar(yytext);
		}
		else if (token < LPAREN || token > RBRACE) {
			errorInvalidToken(token);
		}
		
		if (token == RPAREN || token == RBRACE)
		{
			if (token != brackets.top() + 1) {
				errorBadExpression();
			}
			tab--;
			brackets.pop();
		}
		for (int i = 0; i < tab; i++) {
			cout << TAB;
		}
		cout << yytext << endl;
		if (token == LPAREN || token == LBRACE) {
			tab++;
			brackets.push(token);
		}
	}
	return 0;
}