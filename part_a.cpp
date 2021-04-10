#include "tokens.hpp"
#include <iostream>

int main()
{
	int token;
	while(token = yylex()) {
	// Your code here
	std::cout << token << std::endl;
		}
	return 0;
}