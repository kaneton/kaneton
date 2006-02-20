/* clexer.l: Lex code to help build a C token file.
 * Copyright (c) Warren Toomey, under the GPL license.
 *
 * Much of this file comes from cslang.l, a part of the CSlang program
 * which is Copyright (C) 1995 Tudor Hulubei under the GPL.
 *
 * $Revision: 1.1.1.1 $
 */
%{
#include <string.h>
#include "tokens.h"
#undef yywrap
void myputc(char ch, FILE *f);
void myputid(char ch, char *text, FILE *f);
void myputint(char ch, char *text, FILE *f);
extern FILE *zout;
int inside_comment = 0;

/* We keep a list of each identifier on a per file basis.
 */
#define LIST_SIZE 65536
char *idlist[LIST_SIZE];
%}


%%

break		myputc(BREAK, zout);
case		myputc(CASE, zout);
char		myputc(CHAR, zout);
const		myputc(CONST, zout);
continue	myputc(CONTINUE, zout);
default		myputc(DEFAULT, zout);
do		myputc(DO, zout);
double		myputc(DOUBLE, zout);
else		myputc(ELSE, zout);
enum		myputc(ENUM, zout);
extern		myputc(EXTERN, zout);
float		myputc(FLOAT, zout);
for		myputc(FOR, zout);
goto		myputc(GOTO, zout);
if		myputc(IF, zout);
int		myputc(INT, zout);
long		myputc(LONG, zout);
register	myputc(REGISTER, zout);
return		myputc(RETURN, zout);
short		myputc(SHORT, zout);
signed		myputc(SIGNED, zout);
sizeof		myputc(SIZEOF, zout);
static		myputc(STATIC, zout);
struct		myputc(STRUCT, zout);
switch		myputc(SWITCH, zout);
typedef		myputc(TYPEDEF, zout);
union		myputc(UNION, zout);
unsigned	myputc(UNSIGNED, zout);
void		myputc(VOID, zout);
volatile	myputc(VOLATILE, zout);
while		myputc(WHILE, zout);

#[ \t]*define	myputc(HASHdefine, zout);
#[ \t]*elif	myputc(HASHelif, zout);
#[ \t]*else	myputc(HASHelse, zout);
#[ \t]*endif	myputc(HASHendif, zout);
#[ \t]*error	myputc(HASHerror, zout);
#[ \t]*ifdef	myputc(HASHifdef, zout);
#[ \t]*if	myputc(HASHif, zout);
#[ \t]*ifndef	myputc(HASHifndef, zout);
#[ \t]*include	myputc(HASHinclude, zout);
#[ \t]*line	myputc(HASHline, zout);
#[ \t]*pragma	myputc(HASHpragma, zout);
#[ \t]*undef	myputc(HASHundef, zout);
#[ \t]*warning	myputc(HASHwarning, zout);


"->"		myputc(ARROW, zout);
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
"..."		myputc(ELLIPSIS, zout);
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
"/*"		inside_comment=1;
"*/"		inside_comment=0;

[a-zA-Z_][a-zA-Z_0-9]*		myputid(IDENTIFIER, yytext, zout);

[a-zA-Z_][a-zA-Z_0-9]*:		myputc(LABEL, zout);

0[xX][0-9a-fA-F]+[uUlL]* |
[1-9][0-9]*[uUlL]*	 |
0[0-7]*[uUlL]*			myputint(INTVAL, yytext, zout);

'\\x[0-9a-fA-F]+' |
'\\[0-7]+' 			myputc(CHARCONST, zout);

'.'				myputc(CHARCONST, zout);

'\\.' 				myputc(CHARCONST, zout);

\"[^\"]*\"			myputc(STRINGLIT, zout);

\n				myputc(LINE, zout);

[ \t]*		;	/* Ignore tabs and spaces.  */

.		;	/* Ignore all unrecognised tokens */

%%

void myputc(char ch, FILE *f)
{
  if (inside_comment && (ch!=LINE)) return;
  putc(ch, f);
}

void initialise_lists(void)
{
  memset(idlist, 0, LIST_SIZE);

  /* For various reasons, we don't want to use index 0
   * in this array. So we set up an impossible string
   * to match on, so that it is never used.
   */
  idlist[0]="";
}

/* This function called at the beginning of each input file */
void reset_comments(void)
{
  int i;
  inside_comment= 0;

  /* Free any strings that were strdup()d */
  for (i=1; i<LIST_SIZE; i++) {
    if (idlist[i]) free(idlist[i]);
  }
  initialise_lists();
}

/* Output an identifer token together with its index
 * value from the list.
 */
void myputid(char ch, char *text, FILE *f)
{
  int i;

  /* Output the token to start with */
  if (inside_comment && (ch!=LINE)) return;
  putc(ch, f);

  /* Walk the array to find a match with yytext */
  for (i=0; (i<LIST_SIZE) && (idlist[i]!=NULL); i++) {
    if (!strcmp(idlist[i], text)) break;
  }

  /* 3 possibilities here: we found it when i<LIST_SIZE && idlist[i]!=NULL
   *			   we didn't find it when i<LIST_SIZE && idlist[i]=NULL
   *			   we didn't find it when i==LIST_SIZE
   * For the latter, simply output 0xffff and give up!
   */
  if (i==LIST_SIZE) {
    /* fprintf(stderr, "65535 for %s bad %c\n", text, ch); */
    putc(0xff, f); putc(0xff, f); return;
  }

  /* Found it, output the index value */
  if (idlist[i]!=NULL) {
    /* fprintf(stderr, "%d for %s  %c\n", i, text, ch); */
    putc(i/256, f); putc(i&255, f); return;
  }

  /* Not found, new entry */
  idlist[i]=strdup(text);
  /* fprintf(stderr, "%d for %s new %c\n", i, text, ch); */
  putc(i/256, f); putc(i&255, f);
  return;
}

void myputint(char ch, char *text, FILE *f)
{
  unsigned long num;
  unsigned short i;

  /* Output the token to start with */
  if (inside_comment && (ch!=LINE)) return;
  putc(ch, f);

  num= strtoul(text, NULL, 0);
  i= num & 0xffff;
  putc(i/256, f); putc(i&255, f);
}


int yywrap()
{
    return 1;
}


void dummy()
{
    /* Get rid of the " `yyunput' defined but not used " warning.  */
/*    unput(' ');*/
}
