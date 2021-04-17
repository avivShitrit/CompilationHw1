#include "tokens.hpp"
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::endl;
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
        "SWITCH",
        "CASE",
        "DEFAULT",
        "COLON",
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
        return -1;
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

unsigned int stringLen(const char* str){
    int i=0;
    for (;str[i]!=0;i++);
    return i;
}

void printStringError() {
    unsigned int strlen = stringLen(yytext);
    bool isEscape = false;
    int escapeIndex = -1;
    if(strlen == 0){
        cout<<"Error unclosed string"<<endl;
        return;
    }
    for (size_t i = 0; i < strlen; i++) {
        if (yytext[i] == '\n' || yytext[i] == '\r' || (yytext[i] == '\\' && i == strlen - 2)) {
            cout<< "Error unclosed string"<< endl;
            return;
        }
    }
    for(size_t i = 0; i < strlen; i++){
        if (yytext[i] == '\\') {
            if(yytext[i+1] == 'x'){
                asciiEscapeSequence(i);
                return;
            }
            else if(escapeSequence(yytext[i + 1]) < 0){
                cout<< "Error undefined escape sequence "<< yytext[i + 1]<< endl;
                return;
            }
        }
    }
    cout << "Error unclosed string"<< endl;
}

void endProgram(int status, vector<string>* pointerToDelete= nullptr){
    delete pointerToDelete;
    exit(status);
}

int main()
{
    int token;
    vector<string>* tokenToStr = initTokenToStr();
    while((token = yylex())) {
        unsigned int strlen = stringLen(yytext);
//        //Debug:
//        cout<< "token ="<<token<<endl;
//        if(token>0)
//            cout<<"token str ="<<(*tokenToStr)[token]<<endl;
//        cout<< "yytext ="<<yytext<<endl;
//        cout<<"strlen ="<<strlen<<endl;
//        if(token == STRING && strlen >=2){
//            cout<<"yytext[strlen-2] ="<<yytext[strlen-2]<<endl;
//            cout<<(yytext[strlen-2] == '\\')<<endl;
//        }
        //for STRING token only (can't declare inside switch)
        string outputString;
        switch(token) {
            case WRONGCHAR:
                cout<<"Error "<< yytext<< endl;
                endProgram(0, tokenToStr);
            case WRONGSTRING:
                printStringError();
                endProgram(0, tokenToStr);
            case COMMENT:
                cout<< yylineno<<" "<< (*tokenToStr)[token]<<" "<< "//"<< endl;
                break;
            case STRING:
                if(strlen >=2 && yytext[strlen-2] == '\\'){
                    cout<<"Error unclosed string"<<endl;
                    endProgram(0, tokenToStr);
                }
                for (int i = 0; i < strlen-1; i++) {
                    if (yytext[i] != '\\') {
                        outputString.push_back(yytext[i]);
                    } else {
                        int asciiChar = escapeSequence(yytext[i + 1]);
                        if (asciiChar == -1) {
                            cout<<"Error undefined escape sequence "<< yytext[i + 1]<< endl;
                            endProgram(0, tokenToStr);
                        }
                        if (asciiChar == 'x') {
                            asciiChar = asciiEscapeSequence(i);
                            i += 2;
                            if(asciiChar == -1){
                                endProgram(0, tokenToStr);
                            }
                        }
                        if (asciiChar == 0) {
                            outputString.push_back(asciiChar);
                            i = strlen; //breaking loop (can't use "break" inside switch)
                            continue;
                        }
                        outputString.push_back(asciiChar);
                        i++;
                    }
                }
                cout<< yylineno<<" "<< (*tokenToStr)[token]<<" "<< outputString<< endl;
                break;
            default:
                cout<< yylineno<<" "<< (*tokenToStr)[token]<<" "<< yytext<< endl;
                break;
        }
    }
    return 0;
}