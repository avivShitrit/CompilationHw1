%{
/* need this for the call to atof() below */
%}

%option noyywrap
DIGIT    [0-9]
ID	    [a-z][a-z0-9]*

%%
{DIGIT}+   return NUM;
{ID}	   printf( "An identifier: %s\n", yytext);
.	       printf( "Unrecognized character: %s\n", yytext);
%%