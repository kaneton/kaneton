import sys
sys.path.append('ply')

import lex
import re

data = '''
  time.modtime = strtol(header->fmtime,0, 8);
  time.actime = 0;
  p = (char *)header;
  for (i = 0;
       i < 512 ;
       i++)
    {
      if (i < 148 || i > 155)
	chksum += p[i];
      else
	chksum += '\n';
    }
  if (strtol(header->fchksum, 0, 8) != chksum)
    {
      fprintf(stderr, "Bad checksum archive\n");
      exit(2);
    }
  if (header->ftypeflag == DIRTYPE)
    {
      while (header->fname[strlen(header->fname) - 1] == '/')
	header->fname[strlen(header->fname) - 1] = 0;
      mkdir(header->fname, 0777);
      if(utime(header->fname, &time) != 0)
	perror("utime:");
      if (verbose == 1)
	printf("%s\n", creatpath(header->fname, ""));
      return;
    }
  fd_dest = open(header->fname, O_WRONLY|O_CREAT, 0666);
  size = strtol(header->fsize, 0, 8);
  memset(buffer, 0, 512);
  while (size > 512)
    {
      length = read(fd_tarball, buffer, 512);
      write(fd_dest, buffer, 512);
      size -= 512;
    }
  if(size)
    length = read(fd_tarball, buffer, 512);
  write(fd_dest, buffer, size);
  close(fd_dest);
  if(utime(header->fname, &time) != 0)
    perror("utime:");
  if (verbose == 1)
    printf("%s\n", header->fname);

'''


# List of tokens

reserved = (
    'AUTO', 'BREAK', 'CASE', 'CHAR', 'CONST', 'CONTINUE', 'DEFAULT', 'DO', 'DOUBLE',
    'ELSE', 'ENUM', 'EXTERN', 'FLOAT', 'FOR', 'GOTO', 'IF', 'INT', 'LONG', 'REGISTER',
    'RETURN', 'SHORT', 'SIGNED', 'SIZEOF', 'STATIC', 'STRUCT', 'SWITCH', 'TYPEDEF',
    'UNION', 'UNSIGNED', 'VOID', 'VOLATILE', 'WHILE',
    )

tokens = reserved + (
    'BINOP', # binary operator
    'PREOP', # suffix operator
    'SPEOP', # . or ->
    'IDENT',
    'NUMBER',
    'LPAREN',
    'RPAREN',
    'LBRACKET',
    'RBRACKET',
    'LBRACE',
    'RBRACE',
    'COLON',
    'SEMI',
    'STRING',
    'CHAR',
    'COMMA',
    'CONDOP',
    'SPACES'
)

def t_IDENT(t):
    r'[a-zA-Z_][\w_]*'
    t.type = reserved_map.get(t.value,"IDENT")
    return t

t_NUMBER = r'(0[xX]|)[0-9]+'

t_BINOP = r'(<<=|>>=|<=|>=|==|!=|\|\|<<|>>|&&|\*=|/=|%=|\+=|-=|&=|\|=|\^=|\+|/|\%|&|~|\^|!|<|>|=|\||\*|-)'
t_PREOP = r'(\+\+|--)'
t_SPEOP = r'(\.|->)'

reserved_map = {}
for r in reserved:
    reserved_map[r.lower()] = r

def t_SPACES(t):
    r'\s+'
    t.lineno += t.value.count('\n')
    return t

t_STRING           = r'"(\\.|[^\\"])*"'
t_CHAR             = r'\'(\\.|[^\\\'])\''

t_LPAREN           = r'\('
t_RPAREN           = r'\)'
t_LBRACKET         = r'\['
t_RBRACKET         = r'\]'
t_LBRACE           = r'\{'
t_RBRACE           = r'\}'
t_COMMA            = r','
t_SEMI             = r';'
t_COLON            = r':'

t_CONDOP           = r'\?'
# t_ELLIPSIS         = r'\.\.\.'

def t_status(t):
    print "illegal '%s' line %d" % (t.value[0], t.lineno)
    #exit(1)
    t.skip(1)


lex.lex()

if __name__ == '__main__':
    lex.input(data)
    while 1:
        tok = lex.token()
        if not tok: break      # No more input
        print tok
