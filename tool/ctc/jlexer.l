/* jlexer.l: Lex code to help build a Java token file.
 * Copyright (c) Warren Toomey, under the GPL3 license.
 *
 * Much of this file comes from cslang.l, a part of the CSlang program
 * which is Copyright (C) 1995 Tudor Hulubei under the GPL.
 *
 * This is probably an incomplete lexer for Java; e-mail me improvements!
 *
 * $Revision: 1.2 $
 */
%{
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include "tokens.h"
#undef yywrap
void myputc(char ch, FILE *f);
void myputid(char ch, char *text, FILE *f);
void myputtokhash(char ch, char *text, FILE *f);
extern FILE *zout;
int inside_comment = 0;	/* 1 if inside slash-star, 2 if in slash-slash */

%}


%%

abstract	myputc(ABSTRACT, zout);
boolean		myputc(BOOLEAN, zout);
break		myputc(BREAK, zout);
byte		myputc(BYTE, zout);
case		myputc(CASE, zout);
char		myputc(CHAR, zout);
const		myputc(CONST, zout);
continue	myputc(CONTINUE, zout);
default		myputc(DEFAULT, zout);
do		myputc(DO, zout);
double		myputc(DOUBLE, zout);
else		myputc(ELSE, zout);
extends		myputc(EXTENDS, zout);
enum		myputc(ENUM, zout);
final		myputc(FINAL, zout);
finally		myputc(FINALLY, zout);
float		myputc(FLOAT, zout);
for		myputc(FOR, zout);
goto		myputc(GOTO, zout);
if		myputc(IF, zout);
implements	myputc(IMPLEMENTS, zout);
import		myputc(IMPORT, zout);
instanceof	myputc(INSTANCEOF, zout);
int		myputc(INT, zout);
interface	myputc(INTERFACE, zout);
long		myputc(LONG, zout);
native		myputc(NATIVE, zout);
new		myputc(NEW, zout);
null		myputc(JAVANULL, zout);
package		myputc(PACKAGE, zout);
private		myputc(PRIVATE, zout);
protected	myputc(PROTECTED, zout);
public		myputc(PUBLIC, zout);
return		myputc(RETURN, zout);
short		myputc(SHORT, zout);
static		myputc(STATIC, zout);
strictfp	myputc(STRICTFP, zout);
super		myputc(SUPER, zout);
switch		myputc(SWITCH, zout);
synchronized	myputc(SYNCHRONIZED, zout);
this		myputc(THIS, zout);
throw		myputc(THROW, zout);
throws		myputc(THROWS, zout);
transient	myputc(TRANSIENT, zout);
try		myputc(TRY, zout);
void		myputc(VOID, zout);
volatile	myputc(VOLATILE, zout);
while		myputc(WHILE, zout);


"++"		myputc(INCR, zout);
"--"		myputc(DECR, zout);
"&&"		myputc(ANDAND, zout);
"||"		myputc(OROR, zout);
"+="		myputc(PLUSassign, zout);
"+"		myputc(PLUS, zout);
"-="		myputc(MINUSassign, zout);
"-"		myputc(MINUS, zout);
"*="		myputc(MULTassign, zout);
"*"		myputc(MULT, zout);
"/="		myputc(DIVassign, zout);
"/"		myputc(DIV, zout);
"%="		myputc(MODassign, zout);
"&="		myputc(ANDassign, zout);
"|="		myputc(ORassign, zout);
"^="		myputc(XORassign, zout);
"=="		myputc(EQ, zout);
"!="		myputc(NE, zout);
"<="		myputc(LE, zout);
">="		myputc(GE, zout);
"<<"		myputc(LS, zout);
">>"		myputc(RS, zout);
"<<="		myputc(LSassign, zout);
">>="		myputc(RSassign, zout);
">>>"		myputc(URS, zout);
">>>="		myputc(URSassign, zout);
"("		myputc(OPENPAREN, zout);
")"		myputc(CLOSEPAREN, zout);
"{"		myputc(OPENCURLY, zout);
"}"		myputc(CLOSECURLY, zout);
"["		myputc(OPENBRACKET, zout);
"]"		myputc(CLOSEBRACKET, zout);
"&"		myputc(AND, zout);
","		myputc(COMMA, zout);
";"		myputc(SEMICOLON, zout);
"<"		myputc(LT, zout);
"="		myputc(EQUALS, zout);
">"		myputc(GT, zout);
"?"		myputc(QUESTION, zout);
"!"		myputc(NOT, zout);
":"		myputc(COLON, zout);
"."		myputc(DOT, zout);
"%"		myputc(MOD, zout);
"^"		myputc(CARET, zout);
"|"		myputc(OR, zout);
"~"		myputc(TILDE, zout);
"\\"		myputc(BACKSLASH, zout);
"/*"		{ if (inside_comment==0) inside_comment=1; }
"//"		{ if (inside_comment==0) inside_comment=2; }
"*/"		{ if (inside_comment==1) inside_comment=0; }

[a-zA-Z_][a-zA-Z_0-9]*		myputtokhash(IDENTIFIER, yytext, zout);

[a-zA-Z_][a-zA-Z_0-9]*:		myputtokhash(LABEL, yytext, zout);

0[xX][0-9a-fA-F]+[uUlL]* |
[1-9][0-9]*[uUlL]*	 |
0[0-7]*[uUlL]*			myputtokhash(INTVAL, yytext, zout);

'\\x[0-9a-fA-F]+' |
'\\[0-7]+' 			myputtokhash(CHARCONST, yytext, zout);

'.'				myputtokhash(CHARCONST, yytext, zout);

'\\.' 				myputtokhash(CHARCONST, yytext, zout);

\"[^\"]*\"			myputtokhash(STRINGLIT, yytext, zout);

\n			{ myputc(LINE, zout);
			  if (inside_comment==2) inside_comment=0;
			}

[ \t]*		;	/* Ignore tabs and spaces.  */

.		;	/* Ignore all unrecognised tokens */

%%

void myputc(char ch, FILE *f)
{
  if (inside_comment && (ch!=LINE)) return;
  putc(ch, f);
}


/* CCITT CRC-16 hash:
 * returns a 16-bit number
 * for the input string.
 */
unsigned int get_hashval(char *t)
{
  unsigned int crc=0;

  while (*t!='\0') {
    crc  = (unsigned char)(crc >> 8) | (crc << 8);
    crc ^= *t;
    crc ^= (unsigned char)(crc & 0xff) >> 4;
    crc ^= (crc << 8) << 4;
    crc ^= ((crc & 0xff) << 4) << 1;
    t++;
  }
  /* printf("Got %d for %s\n", h, oldt); */
  return(crc & 0xffff);
}


/* This function called at the beginning of each input file */
void reset_comments(void)
{
  inside_comment= 0;
}


/* Given a STRINGLIT, CHARCONST, INTVAL, IDENTIFIER or LABEL,
 * output the token followed by a 16-bit hash of the value.
 */
void myputtokhash(char ch, char *text, FILE *f)
{
  unsigned int hash;

  /* Output the token to start with */
  if (inside_comment) return;
  putc(ch, f);

  hash= get_hashval(text);
  /* printf("Hash %04x %s\n", hash, text); */
  putc(hash/256, f); putc(hash&255, f); return;
}


int yywrap()
{
    return 1;
}


void dummy()
{
    /* Get rid of the " `yyunput' defined but not used " warning.  */
    unput(' ');
}


/*
 * Is the specified file eligible to be compared?
 * This code comes from Eric Raymond's comparator 1.0 source code,
 * which is (c) Eric Raymond under the GPL.
 */
FILE * eligible(const char *file)
{
   FILE *zin;

  /* fast check for the most common suffixes */
#define endswith(suff)	!strcmp(suff, file + strlen(file) - strlen(suff))
  if (endswith(".o") || endswith("~"))
    return (NULL);
  else if (strstr(file, "CVS") || strstr(file, "RCS") || strstr(file, "SCCS"))
    return (NULL);

  zin = fopen(file, "r");
  if (zin == NULL) return (NULL);
  if (endswith(".java") || endswith(".JAVA"))
    return (zin);

  fclose(zin); return (NULL);
#undef endswith
}
