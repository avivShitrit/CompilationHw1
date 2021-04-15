#include "tokens.hpp"
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout, std::endl;
vector<string>* initTokenToStr(){
	auto tokenToStr = new vector<string> {
		"",
		"VOID",
		"INT",
		"BYTE",
		"B",
		"BOOL",
		"AND",
		"OR",
		"NOT",
		"TRUE",
		"FALSE",
		"RETURN",
		"IF",
		"ELSE",
		"WHILE",
		"BREAK",
		"CONTINUE",
		"SC",
		"COMMA",
		"LPAREN",
		"RPAREN",
		"LBRACE",
		"RBRACE",
		"ASSIGN",
		"RELOP",
		"BINOP",
		"COMMENT",
		"ID",
		"NUM",
		"STRING",
	};
	return tokenToStr;
}

char fromHexToDecAscii(char i,char j) {
    if (j != 0) {
        if ((i >= '0' && i <= '7')) {
            int first = i - '0';
            int second = -1;
            if (j >= '0' && j <= '9') {
                second = j - '0';
            } else if ((j >= 'a' && j <= 'f')) {
                second = j - 'a' + 10;
            } else if ((j >= 'A' && j <= 'F')) {
                second = j - 'A' + 10;
            }
            return first * 16 + second;
        }
    }
    return -1;
}

int asciiEscapeSequence(int startIndex){
    int ascii_char = fromHexToDecAscii(yytext[startIndex + 2], yytext[startIndex + 3]);
    if (ascii_char == -1) {
        printf("Error undefined escape sequence x");
        if (yytext[startIndex + 2] == '\"') {
            std::cout << std::endl;
        } else if (yytext[startIndex + 3] == '\"') {
            std::cout << yytext[startIndex + 2] << std::endl;
        } else {
            std::cout << yytext[startIndex + 2] << yytext[startIndex + 3] << std::endl;
        }
        exit(0);
    }
    return ascii_char;
}

char escapeSequence(char c){
    switch (c) {
        case 'n':
            return '\n';
        case '0':
            return '\0';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case '\\':
            return '\\';
        case '"':
            return '"';
        case 'x':
            return 'x';
        default:
            return -1;
    }
}

//Debug
unsigned int stringLen(const char* str){
    int i=0;
    for (;str[i]!=0;i++);
    return i;
}

void findStringError() {
    unsigned int strlen = stringLen(yytext);
    //Debug:
    if(strlen != yyleng){
        printf("strlen (%d) != yyleng (%d)", strlen, yyleng);
        exit(0);
    }
    cout<<"no problem with yyleng"<<endl;
    if(strlen == 0){
        cout<<"Error unclosed string\n";
        exit(0);
    }
    for (size_t i = 0; i < strlen; i++) {
        if (yytext[i] == '\n' || yytext[i] == '\r' || (yytext[i] == '\\' && i == strlen - 2)) {
            cout<< "Error unclosed string"<< endl;
            exit(0);
        }
        if (yytext[i] == '\\') {
            if (yytext[i + 1] == 'x') {
                asciiEscapeSequence(i);
            }
            cout<< "Error undefined escape sequence "<< yytext[i + 1]<< endl;
            exit(0);
        }
        if (yytext[i] == '\"') {
            cout<< "Error "<< yytext[i]<< endl;
            exit(0);
        }
    }
    cout<< "Error unclosed string"<< endl;
    exit(0);
}



int main()
{
    int token;
    vector<string>* tokenToStr = initTokenToStr();
    string tokenStr = (*tokenToStr)[token];
    while((token = yylex())) {
        //for STRING token only (can't declare inside switch)
        string outputString;
        switch(token) {
            //unsupported char
            case -1:
                cout << "Error " << yytext << endl;
                exit(0);
                break;
            //wrong string
            case -2:
                findStringError();
                break;
            case COMMENT:
                cout<< yylineno<< tokenStr<< "//";
                break;
            case STRING:
                for (int i = 0; i < yyleng - 1; i++) {
                    if (yytext[i] != '\\') {
                        outputString.push_back(yytext[i]);
                    } else {
                        int asciiChar = escapeSequence(yytext[i + 1]);
                        if (asciiChar == -1) {
                            printf("Error undefined escape sequence %c\n", yytext[i + 1]);
                            exit(0);
                        }
                        if (asciiChar == 'x') {
                            asciiChar = asciiEscapeSequence(i);
                            i += 2;
                        }
                        if (asciiChar == 0) {
                            outputString.push_back(asciiChar);
                            i = yyleng-1; //breaking loop (can't use "break" inside switch)
                            continue;
                        }
                        outputString.push_back(asciiChar);
                        i++;
                    }
                }
                cout<< yylineno<< tokenStr<< outputString;
                break;
            default:
                cout<< yylineno<< tokenStr<< yytext;
                break;
        }
    }
	return 0;
}