%{
/* need this for the call to atof() below */
#include "tokens.hpp"
%}

%option noyywrap
%option yylineno

NUM     [0]|([1-9][0-9]*)
ID	    [A-Za-z][A-Za-z0-9]*
RELOP   ==|!=|<=|>=|<|>
BINOP   \+|\-|\*|\/
COMMENT (\/\/[^\n\r]*)
%x STRINGS

%%
void        return VOID;
int         return INT;
byte        return BYTE;
b           return B;
bool        return BOOL;
and         return AND;
or          return OR;
not         return NOT;
true        return TRUE;
false       return FALSE;
return      return RETURN;
if          return IF;
else        return ELSE;
while       return WHILE;
break       return BREAK;
continue    return CONTINUE;
switch      return SWITCH;
case        return CASE;
default     return DEFAULT;
(\:)        return COLON;
(\;)        return SC;
(\,)        return COMMA;
(\()        return LPAREN;
(\))        return RPAREN;
(\{)        return LBRACE;
(\})        return RBRACE;
=           return ASSIGN;
{RELOP}     return RELOP;
{BINOP}     return BINOP;
{COMMENT}   return COMMENT;
{ID}        return ID;
{NUM}       return NUM;
(\")        BEGIN(STRINGS);
<STRINGS><<EOF>>    return -2;
<STRINGS>([\x20-\x21\x23-\x2e\x30-\x7e]|((\\)(\\))|((\\)(\"))|((\\)(n))|((\\)(r))|((\\)(t))|((\\)(0))|((\\)x))*(\") {BEGIN(INITIAL);return STRING;}
<STRINGS>([^(\")])*((\")?)  return -2;
.           return -1;

%%