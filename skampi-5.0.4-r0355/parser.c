/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     STRING = 259,
     INTEGER = 260,
     DOUBLE = 261,
     BEGIN_TOKEN = 262,
     END = 263,
     MEASUREMENT = 264,
     DIAGRAM = 265,
     RESULT = 266,
     TO = 267,
     STEP = 268,
     METHOD = 269,
     PRINT = 270,
     PRINTALL = 271,
     FLUSH = 272,
     FOR = 273,
     DO = 274,
     OD = 275,
     IN = 276,
     MEASURE = 277,
     IF = 278,
     THEN = 279,
     ELSE = 280,
     FI = 281,
     EQ = 282,
     NEQ = 283,
     LE = 284,
     GE = 285,
     AND = 286,
     OR = 287,
     NOT = 288,
     NOTLIMITED = 289,
     MULTIPLEOF = 290,
     AUTOREFINE = 291,
     ILLEGALCHAR = 292
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define STRING 259
#define INTEGER 260
#define DOUBLE 261
#define BEGIN_TOKEN 262
#define END 263
#define MEASUREMENT 264
#define DIAGRAM 265
#define RESULT 266
#define TO 267
#define STEP 268
#define METHOD 269
#define PRINT 270
#define PRINTALL 271
#define FLUSH 272
#define FOR 273
#define DO 274
#define OD 275
#define IN 276
#define MEASURE 277
#define IF 278
#define THEN 279
#define ELSE 280
#define FI 281
#define EQ 282
#define NEQ 283
#define LE 284
#define GE 285
#define AND 286
#define OR 287
#define NOT 288
#define NOTLIMITED 289
#define MULTIPLEOF 290
#define AUTOREFINE 291
#define ILLEGALCHAR 292




/* Copy the first part of user declarations.  */
#line 21 "parser.y"


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#include "misc.h"
#include "debug.h"
#include "output.h"
#include "main.h"
#include "private_misc.h"
#include "exec.h"
#include "print.h"
#include "symboltable.h"
#include "compile.h"

struct loop_variable {
  struct variable *variable;
  bool refine;
};


extern int yylex(void); /* @@ actually one should include some header file */
                        /*    created by flex */


int current_line_number;
char *current_filename;
char *input_filename;
char *input_path;
int char_counter;

char *measurement_name;

void print_source(int from, int to)
{
  int i;
  int newline;
  char c;
  
  /* move to first non-whitespace line */
  int line_start = from;
  i = from;
  while (i < to) {
    c = source_buf.srcbuf[i];
    if (c == '\n') line_start = i+1;
    if (!(c == '\n' || c == '\t' || c == ' '))
      break;
    i++;
  }
  from = line_start;
  
  newline = 1;
  for (i=from; i<to; i++) {
    if (newline) {
      print_output("#> ");
      newline = 0;
    }
    print_output("%c", source_buf.srcbuf[i]);
    if (source_buf.srcbuf[i] == '\n') {
      newline = 1;
    }
  }
  print_output("\n\n");
}

void store_measurement_pos()
{
  if (source_buf.max_measurements <= source_buf.num_measurements) {
    source_buf.measurement_pos = realloc(source_buf.measurement_pos, source_buf.max_measurements*2);
    source_buf.max_measurements *= 2;
  }
  source_buf.measurement_pos[source_buf.num_measurements] = char_counter;
  source_buf.num_measurements++;
}

void yyerror(char *s)
{
  if( grootproc() ) fprintf(stderr, "%s in line %d\n", s, current_line_number); 
  parser_result = False;
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 107 "parser.y"
{
  char *identifier;
  int intv;
  double doublev;
  char *stringv;
  struct symboltable_entry *symboltable_entry;
  struct variable *variable;
  struct loop_variable *loop_variable;
  struct term *term;
  struct statement *statement;
}
/* Line 187 of yacc.c.  */
#line 268 "parser.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 281 "parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   493

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNRULES -- Number of states.  */
#define YYNSTATES  147

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    46,    39,     2,
      52,    53,    44,    42,    54,    43,     2,    45,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    49,     2,
      40,    48,    41,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,    38,    56,    47,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    13,    15,    19,
      21,    23,    26,    29,    31,    36,    44,    54,    65,    78,
      86,    95,   101,   109,   113,   117,   121,   123,   127,   128,
     130,   134,   139,   141,   143,   145,   147,   151,   155,   159,
     163,   167,   171,   175,   178,   181,   183,   187,   191,   195,
     199,   203,   207,   211,   215,   219,   223,   226,   229,   231,
     233,   235,   236,   244,   247,   249,   252,   254
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      74,     0,    -1,    63,    -1,    58,    63,    -1,    -1,    58,
      -1,    61,    -1,    62,    -1,     3,    48,    70,    -1,    68,
      -1,    60,    -1,    15,    66,    -1,    16,    66,    -1,    17,
      -1,    22,    70,    49,    70,    -1,    64,    70,    12,    69,
      19,    59,    20,    -1,    64,    70,    12,    69,    13,    70,
      19,    59,    20,    -1,    64,    70,    12,    69,    13,    44,
      70,    19,    59,    20,    -1,    64,    70,    12,    69,    13,
      44,    70,    35,    70,    19,    59,    20,    -1,    65,    50,
      67,    51,    19,    59,    20,    -1,    65,    75,    52,    67,
      53,    19,    59,    20,    -1,    23,    70,    24,    59,    26,
      -1,    23,    70,    24,    59,    25,    59,    26,    -1,    18,
       3,    48,    -1,    36,     3,    48,    -1,    18,     3,    21,
      -1,    70,    -1,    67,    54,    70,    -1,    -1,    70,    -1,
      67,    54,    70,    -1,     3,    52,    67,    53,    -1,    70,
      -1,    34,    -1,    71,    -1,     3,    -1,    55,    67,    56,
      -1,    70,    42,    70,    -1,    70,    43,    70,    -1,    70,
      44,    70,    -1,    70,    45,    70,    -1,    70,    46,    70,
      -1,    52,    70,    53,    -1,    43,    70,    -1,    42,    70,
      -1,    68,    -1,    70,    40,    70,    -1,    70,    41,    70,
      -1,    70,    27,    70,    -1,    70,    28,    70,    -1,    70,
      29,    70,    -1,    70,    30,    70,    -1,    70,    31,    70,
      -1,    70,    39,    70,    -1,    70,    32,    70,    -1,    70,
      38,    70,    -1,    33,    70,    -1,    47,    70,    -1,     5,
      -1,     6,    -1,     4,    -1,    -1,     7,     9,     4,    73,
      59,     8,     9,    -1,    74,    72,    -1,    72,    -1,    74,
      60,    -1,    60,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   152,   152,   156,   166,   167,   170,   170,   172,   178,
     186,   187,   192,   197,   203,   209,   215,   221,   227,   232,
     236,   240,   244,   249,   255,   262,   267,   272,   281,   282,
     286,   294,   320,   324,   329,   334,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   370,   375,
     380,   387,   386,   418,   419,   420,   422,   426
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "STRING", "INTEGER",
  "DOUBLE", "BEGIN_TOKEN", "END", "MEASUREMENT", "DIAGRAM", "RESULT", "TO",
  "STEP", "METHOD", "PRINT", "PRINTALL", "FLUSH", "FOR", "DO", "OD", "IN",
  "MEASURE", "IF", "THEN", "ELSE", "FI", "EQ", "NEQ", "LE", "GE", "AND",
  "OR", "NOT", "NOTLIMITED", "MULTIPLEOF", "AUTOREFINE", "ILLEGALCHAR",
  "'|'", "'&'", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "'~'",
  "'='", "':'", "'['", "']'", "'('", "')'", "','", "'{'", "'}'", "$accept",
  "meas_statement_list", "meas_statement_list0", "simple_statement",
  "assignment", "procedure_call", "meas_statement", "for_head",
  "for_in_head", "term_list1", "term_list", "function_call",
  "for_end_term", "term", "value", "measurement", "@1", "measurement_list",
  "iterator", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   124,    38,
      60,    62,    43,    45,    42,    47,    37,   126,    61,    58,
      91,    93,    40,    41,    44,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    57,    58,    58,    59,    59,    60,    60,    61,    62,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    64,    64,    65,    66,    66,    67,    67,
      67,    68,    69,    69,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    71,    71,
      71,    73,    72,    74,    74,    74,    74,    75
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     3,     1,
       1,     2,     2,     1,     4,     7,     9,    10,    12,     7,
       8,     5,     7,     3,     3,     3,     1,     3,     0,     1,
       3,     4,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     1,     1,
       1,     0,     7,     2,     1,     2,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,    66,     6,     7,     9,    64,     0,     0,
      28,     0,     1,    65,    63,    35,    60,    58,    59,     0,
       0,     0,     0,     0,    28,    45,     8,    34,     0,    29,
      61,    56,    44,    43,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    31,     0,     4,    42,    36,    48,    49,    50,
      51,    52,    54,    55,    53,    46,    47,    37,    38,    39,
      40,    41,    30,    28,    28,    13,     0,     0,     0,     0,
       5,     0,    10,     2,     0,     0,    11,     0,    26,    12,
       0,     0,     0,     0,     3,     0,     0,    67,    28,     0,
       0,    25,    23,     0,     4,    24,    62,     0,     0,    28,
      27,    14,     0,    33,     0,    32,     0,     0,     4,    21,
       0,     4,     4,     0,     0,     0,     0,     0,     0,     4,
      22,     0,     4,    15,    19,     0,     4,     0,     0,    20,
       0,     0,    16,    17,     4,     0,    18
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    80,    81,    82,     4,     5,    83,    84,    85,    86,
      87,    25,   114,    29,    27,     7,    54,     8,    99
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -32
static const yytype_int16 yypact[] =
{
       3,   -30,    10,   -32,   -32,   -32,   -32,   -32,    46,   120,
     120,     5,   -32,   -32,   -32,   -28,   -32,   -32,   -32,   120,
     120,   120,   120,   120,   120,   -32,   362,   -32,    33,   362,
     -32,   -32,     6,     6,   -32,   299,    19,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   -32,   120,   181,   -32,   -32,   447,   447,    39,
      39,   402,   382,   422,   429,    39,    39,     6,     6,   -32,
     -32,   -32,   362,   120,   120,   -32,    27,   120,   120,    60,
     181,    58,   -32,   -32,   120,    -2,   -32,    25,   203,   -32,
     -19,   319,   342,    28,   -32,    61,   149,   -32,   120,    36,
     120,   -32,   -32,   120,   181,   -32,   -32,    22,   -31,   120,
     223,   362,    32,   -32,    -8,   362,    73,    37,   181,   -32,
      56,   181,   181,    74,    70,   120,   251,    82,    85,   181,
     -32,   183,   181,   -32,   -32,    87,   181,   120,    89,   -32,
      90,   279,   -32,   -32,   181,    92,   -32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -32,   -32,    16,     4,   -32,   -32,    35,   -32,   -32,    42,
      -3,     0,   -32,    -6,   -32,   105,   -32,   -32,   -32
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -31
static const yytype_int16 yytable[] =
{
       6,    97,   101,    26,     3,   120,     1,    28,     6,    30,
       2,   121,    13,    31,    32,    33,    34,    35,     9,    11,
     116,    36,    10,    53,    10,    15,    16,    17,    18,   102,
      90,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    12,    72,    98,     1,
      49,    50,    51,     2,     6,    19,   113,   118,   119,    15,
      16,    17,    18,    93,    20,    21,    95,    88,    88,    22,
     106,    91,    92,    53,    23,    56,   105,    24,    96,   100,
       6,    47,    48,    49,    50,    51,    52,    53,   109,    19,
     123,    53,   122,   129,   110,   108,   130,   111,    20,    21,
     125,   115,   133,    22,     6,   134,   117,   139,    23,   142,
     143,    24,   146,    14,   126,    94,    89,     0,     6,   131,
     112,     6,     6,    15,    16,    17,    18,     0,     0,     6,
       0,   141,     6,     0,   124,     0,     6,   127,   128,     0,
       0,     0,     0,     0,     6,   135,     0,     0,   138,     0,
       0,     0,   140,    19,     0,     0,     0,     0,     0,     0,
     145,   107,    20,    21,     0,     0,     0,    22,     0,     0,
       0,     0,    23,     0,     0,    24,    37,    38,    39,    40,
      41,    42,     0,     0,     1,     0,     0,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    73,    74,    75,    76,
       0,     0,   136,    77,    78,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,     0,    79,   137,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      37,    38,    39,    40,    41,    42,     0,     0,     0,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      37,    38,    39,    40,    41,    42,     0,   -29,     0,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
     132,     0,     0,     0,     0,     0,     0,   -30,    37,    38,
      39,    40,    41,    42,     0,     0,     0,     0,     0,    43,
      44,    45,    46,    47,    48,    49,    50,    51,   144,     0,
       0,     0,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,     0,     0,     0,     0,     0,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    37,    38,    39,    40,
      41,    42,     0,     0,     0,     0,     0,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    37,    38,    39,    40,
      41,    42,    55,     0,     0,     0,     0,    43,    44,    45,
      46,    47,    48,    49,    50,    51,   104,     0,   103,    37,
      38,    39,    40,    41,    42,     0,     0,     0,     0,     0,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    37,
      38,    39,    40,    41,    42,     0,     0,     0,     0,     0,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    37,
      38,    39,    40,    41,     0,     0,     0,     0,     0,     0,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    37,
      38,    39,    40,     0,     0,     0,     0,     0,     0,     0,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    37,
      38,    39,    40,     0,     0,     0,    37,    38,    39,    40,
       0,    44,    45,    46,    47,    48,    49,    50,    51,    45,
      46,    47,    48,    49,    50,    51,    39,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    45,    46,    47,
      48,    49,    50,    51
};

static const yytype_int16 yycheck[] =
{
       0,     3,    21,     9,     0,    13,     3,    10,     8,     4,
       7,    19,     8,    19,    20,    21,    22,    23,    48,     9,
      51,    24,    52,    54,    52,     3,     4,     5,     6,    48,
       3,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,     0,    53,    50,     3,
      44,    45,    46,     7,    54,    33,    34,    25,    26,     3,
       4,     5,     6,     3,    42,    43,     8,    73,    74,    47,
       9,    77,    78,    54,    52,    56,    48,    55,    84,    54,
      80,    42,    43,    44,    45,    46,    53,    54,    52,    33,
      53,    54,    19,    19,   100,    98,    26,   103,    42,    43,
      44,   107,    20,    47,   104,    20,   109,    20,    52,    20,
      20,    55,    20,     8,   120,    80,    74,    -1,   118,   125,
     104,   121,   122,     3,     4,     5,     6,    -1,    -1,   129,
      -1,   137,   132,    -1,   118,    -1,   136,   121,   122,    -1,
      -1,    -1,    -1,    -1,   144,   129,    -1,    -1,   132,    -1,
      -1,    -1,   136,    33,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    12,    42,    43,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    52,    -1,    -1,    55,    27,    28,    29,    30,
      31,    32,    -1,    -1,     3,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    15,    16,    17,    18,
      -1,    -1,    19,    22,    23,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    30,    31,    32,    -1,    36,    35,    -1,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      27,    28,    29,    30,    31,    32,    -1,    54,    -1,    -1,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    54,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    27,    28,    29,    30,
      31,    32,    53,    -1,    -1,    -1,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    24,    -1,    49,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    27,
      28,    29,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    27,
      28,    29,    30,    -1,    -1,    -1,    27,    28,    29,    30,
      -1,    39,    40,    41,    42,    43,    44,    45,    46,    40,
      41,    42,    43,    44,    45,    46,    29,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
      43,    44,    45,    46
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     7,    60,    61,    62,    68,    72,    74,    48,
      52,     9,     0,    60,    72,     3,     4,     5,     6,    33,
      42,    43,    47,    52,    55,    68,    70,    71,    67,    70,
       4,    70,    70,    70,    70,    70,    67,    27,    28,    29,
      30,    31,    32,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    53,    54,    73,    53,    56,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    15,    16,    17,    18,    22,    23,    36,
      58,    59,    60,    63,    64,    65,    66,    67,    70,    66,
       3,    70,    70,     3,    63,     8,    70,     3,    50,    75,
      54,    21,    48,    49,    24,    48,     9,    12,    67,    52,
      70,    70,    59,    34,    69,    70,    51,    67,    25,    26,
      13,    19,    19,    53,    59,    44,    70,    59,    59,    19,
      26,    70,    19,    20,    20,    59,    19,    35,    59,    20,
      59,    70,    20,    20,    19,    59,    20
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 153 "parser.y"
    { 
  (yyval.statement) = (yyvsp[(1) - (1)].statement);
;}
    break;

  case 3:
#line 157 "parser.y"
    { 
  struct statement *s;
  iodebug(DBG_PARSER2, "meas_statement_list: meas_statement_list meas_statement\n");
  for( s = (yyvsp[(1) - (2)].statement); s->next != NULL ; s = s->next ) ;
  s->next = (yyvsp[(2) - (2)].statement);
  (yyval.statement) = (yyvsp[(1) - (2)].statement);
;}
    break;

  case 4:
#line 166 "parser.y"
    { (yyval.statement) = NULL; ;}
    break;

  case 5:
#line 168 "parser.y"
    { (yyval.statement) = (yyvsp[(1) - (1)].statement); ;}
    break;

  case 6:
#line 170 "parser.y"
    { (yyval.statement) = (yyvsp[(1) - (1)].statement); ;}
    break;

  case 7:
#line 170 "parser.y"
    { (yyval.statement) = (yyvsp[(1) - (1)].statement); ;}
    break;

  case 8:
#line 173 "parser.y"
    { 
  iodebug(DBG_PARSER2, "assignment: IDENTIFIER = term\n");
  (yyval.statement) = create_assignment(search_lvalue((yyvsp[(1) - (3)].identifier)), (yyvsp[(3) - (3)].term));  
;}
    break;

  case 9:
#line 179 "parser.y"
    {
  iodebug(DBG_PARSER2, "procedure_call: '%s'\n", search_fname((yyvsp[(1) - (1)].term)->call_fp, (yyvsp[(1) - (1)].term)->iterator_fp)); 
  (yyval.statement) = create_procedure_call((yyvsp[(1) - (1)].term));
;}
    break;

  case 10:
#line 186 "parser.y"
    { (yyval.statement) = (yyvsp[(1) - (1)].statement); ;}
    break;

  case 11:
#line 188 "parser.y"
    {
  iodebug(DBG_PARSER2, "PRINT term\n");
  (yyval.statement) = create_print((yyvsp[(2) - (2)].term), 0);
;}
    break;

  case 12:
#line 193 "parser.y"
    {
  iodebug(DBG_PARSER2, "PRINTALL term\n");
  (yyval.statement) = create_print((yyvsp[(2) - (2)].term), 1);
;}
    break;

  case 13:
#line 198 "parser.y"
    {
  iodebug(DBG_PARSER2, "FLUSH\n");
  (yyval.statement) = create_flush();
;}
    break;

  case 14:
#line 204 "parser.y"
    {
  iodebug(DBG_PARSER2, "MEASURE term\n");
  (yyval.statement) = create_measure((yyvsp[(2) - (4)].term), (yyvsp[(4) - (4)].term));
;}
    break;

  case 15:
#line 210 "parser.y"
    {
  (yyval.statement) = create_for_loop((yyvsp[(1) - (7)].loop_variable)->variable, (yyvsp[(1) - (7)].loop_variable)->refine, (yyvsp[(2) - (7)].term), (yyvsp[(4) - (7)].term), (yyvsp[(6) - (7)].statement));
  free((yyvsp[(1) - (7)].loop_variable));
;}
    break;

  case 16:
#line 216 "parser.y"
    {
  (yyval.statement) = create_for_step_loop((yyvsp[(1) - (9)].loop_variable)->variable, (yyvsp[(1) - (9)].loop_variable)->refine, (yyvsp[(2) - (9)].term), (yyvsp[(4) - (9)].term), OP_ADD, (yyvsp[(6) - (9)].term), (yyvsp[(8) - (9)].statement));
  free((yyvsp[(1) - (9)].loop_variable));
;}
    break;

  case 17:
#line 222 "parser.y"
    {
  (yyval.statement) = create_for_step_loop((yyvsp[(1) - (10)].loop_variable)->variable, (yyvsp[(1) - (10)].loop_variable)->refine, (yyvsp[(2) - (10)].term), (yyvsp[(4) - (10)].term), OP_MULT,  (yyvsp[(7) - (10)].term), (yyvsp[(9) - (10)].statement));
  free((yyvsp[(1) - (10)].loop_variable));
;}
    break;

  case 18:
#line 228 "parser.y"
    {
  (yyval.statement) = create_for_step_multipleof_loop((yyvsp[(1) - (12)].loop_variable)->variable, (yyvsp[(1) - (12)].loop_variable)->refine, (yyvsp[(2) - (12)].term), (yyvsp[(4) - (12)].term), OP_MULT,  (yyvsp[(7) - (12)].term), (yyvsp[(9) - (12)].term), (yyvsp[(11) - (12)].statement));
  free((yyvsp[(1) - (12)].loop_variable));
;}
    break;

  case 19:
#line 233 "parser.y"
    {
  (yyval.statement) = create_for_list_loop((yyvsp[(1) - (7)].variable), (yyvsp[(3) - (7)].term), (yyvsp[(6) - (7)].statement));
;}
    break;

  case 20:
#line 237 "parser.y"
    {
  (yyval.statement) = create_for_iterator_loop((yyvsp[(1) - (8)].variable), (yyvsp[(2) - (8)].symboltable_entry), (yyvsp[(4) - (8)].term), (yyvsp[(7) - (8)].statement));
;}
    break;

  case 21:
#line 241 "parser.y"
    {
  (yyval.statement) = create_if_then_else((yyvsp[(2) - (5)].term), (yyvsp[(4) - (5)].statement), NULL);
;}
    break;

  case 22:
#line 245 "parser.y"
    {
  (yyval.statement) = create_if_then_else((yyvsp[(2) - (7)].term), (yyvsp[(4) - (7)].statement), (yyvsp[(6) - (7)].statement));
;}
    break;

  case 23:
#line 250 "parser.y"
    {
  (yyval.loop_variable) = malloc(sizeof(struct loop_variable));
  (yyval.loop_variable)->variable = search_lvalue((yyvsp[(2) - (3)].identifier));
  (yyval.loop_variable)->refine = 0;
;}
    break;

  case 24:
#line 256 "parser.y"
    {
  (yyval.loop_variable) = malloc(sizeof(struct loop_variable));
  (yyval.loop_variable)->variable = search_lvalue((yyvsp[(2) - (3)].identifier));
  (yyval.loop_variable)->refine = 1;
;}
    break;

  case 25:
#line 263 "parser.y"
    {
  (yyval.variable) = search_lvalue((yyvsp[(2) - (3)].identifier));
;}
    break;

  case 26:
#line 268 "parser.y"
    {
  (yyval.term) = (yyvsp[(1) - (1)].term);
;}
    break;

  case 27:
#line 273 "parser.y"
    { 
  struct term *t;
  for( t = (yyvsp[(1) - (3)].term); t->next != NULL ; t = t->next ) ;
  t->next = (yyvsp[(3) - (3)].term);
  (yyval.term) = (yyvsp[(1) - (3)].term);
;}
    break;

  case 28:
#line 281 "parser.y"
    { (yyval.term) = NULL; ;}
    break;

  case 29:
#line 283 "parser.y"
    {
  (yyval.term) = (yyvsp[(1) - (1)].term);
;}
    break;

  case 30:
#line 287 "parser.y"
    {
  struct term *t;
  for( t = (yyvsp[(1) - (3)].term); t->next != NULL ; t = t->next ) ;
  t->next = (yyvsp[(3) - (3)].term);
  (yyval.term) = (yyvsp[(1) - (3)].term);
;}
    break;

  case 31:
#line 295 "parser.y"
    {
  int n, interface_length;
  struct term *t;
  struct symboltable_entry *se;

  iodebug(DBG_PARSER2, "function_call: '%s' (", (yyvsp[(1) - (4)].identifier));

  se = search_function((yyvsp[(1) - (4)].identifier));

  if( se == NULL ) 
    error_with_abort(5, "unknown function name '%s' in line %d\n", 
		     (yyvsp[(1) - (4)].identifier), current_line_number);

  for( n = 0, t = (yyvsp[(3) - (4)].term); t != NULL; n++, t = t->next ) ; 
  
  interface_length = strlen(se->interface);
  if( n < interface_length ) 
    error_with_abort(7, "too few parameters for function '%s' in line %d, expecting %d instead of %d\n",
		     (yyvsp[(1) - (4)].identifier), current_line_number, interface_length, n);
  else if( n > interface_length ) 
    error_with_abort(6, "too many parameters for function '%s' in line %d, expecting %d instead of %d\n", 
		     (yyvsp[(1) - (4)].identifier), current_line_number, interface_length, n);
  (yyval.term) = create_function_call(se, (yyvsp[(3) - (4)].term));
;}
    break;

  case 32:
#line 321 "parser.y"
    {
  (yyval.term) = (yyvsp[(1) - (1)].term);
;}
    break;

  case 33:
#line 325 "parser.y"
    {
  (yyval.term) = NULL;
;}
    break;

  case 34:
#line 330 "parser.y"
    { 
  iodebug(DBG_PARSER2, "term: value\n");
  (yyval.term) = (yyvsp[(1) - (1)].term);
;}
    break;

  case 35:
#line 335 "parser.y"
    {
  struct variable *v;


  v = search_rvalue((yyvsp[(1) - (1)].identifier));
  if( v == NULL ) 
    error_with_abort(9, "unknown identifier '%s' in line %d\n", (yyvsp[(1) - (1)].identifier), current_line_number);
  else
    iodebug(DBG_PARSER2, "term: identifier '%s {%s}'\n", (yyvsp[(1) - (1)].identifier), var_type_to_string(v->type));

  (yyval.term) = create_identifier_term(v);
;}
    break;

  case 36:
#line 347 "parser.y"
    { (yyval.term) = create_array_term((yyvsp[(2) - (3)].term)); ;}
    break;

  case 37:
#line 348 "parser.y"
    {  (yyval.term) = create_binary_term(OP_ADD, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 38:
#line 349 "parser.y"
    {  (yyval.term) = create_binary_term(OP_SUB, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 39:
#line 350 "parser.y"
    {  (yyval.term) = create_binary_term(OP_MULT, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 40:
#line 351 "parser.y"
    {  (yyval.term) = create_binary_term(OP_DIV, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 41:
#line 352 "parser.y"
    {  (yyval.term) = create_binary_term(OP_MOD, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 42:
#line 353 "parser.y"
    {  (yyval.term) = (yyvsp[(2) - (3)].term); ;}
    break;

  case 43:
#line 354 "parser.y"
    { (yyval.term) = create_unary_term(OP_SUB, (yyvsp[(2) - (2)].term)); ;}
    break;

  case 44:
#line 355 "parser.y"
    { (yyval.term) = (yyvsp[(2) - (2)].term); ;}
    break;

  case 45:
#line 356 "parser.y"
    { (yyval.term) = (yyvsp[(1) - (1)].term); ;}
    break;

  case 46:
#line 357 "parser.y"
    { (yyval.term) = create_binary_term(OP_LT, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 47:
#line 358 "parser.y"
    { (yyval.term) = create_binary_term(OP_GT, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 48:
#line 359 "parser.y"
    { (yyval.term) = create_binary_term(OP_EQ, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 49:
#line 360 "parser.y"
    { (yyval.term) = create_binary_term(OP_NEQ, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 50:
#line 361 "parser.y"
    { (yyval.term) = create_binary_term(OP_LE, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 51:
#line 362 "parser.y"
    { (yyval.term) = create_binary_term(OP_GE, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 52:
#line 363 "parser.y"
    { (yyval.term) = create_binary_term(OP_AND, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 53:
#line 364 "parser.y"
    { (yyval.term) = create_binary_term(OP_BAND, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 54:
#line 365 "parser.y"
    { (yyval.term) = create_binary_term(OP_OR, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 55:
#line 366 "parser.y"
    { (yyval.term) = create_binary_term(OP_BOR, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 56:
#line 367 "parser.y"
    { (yyval.term) = create_unary_term(OP_NOT, (yyvsp[(2) - (2)].term)); ;}
    break;

  case 57:
#line 368 "parser.y"
    { (yyval.term) = create_unary_term(OP_BNOT, (yyvsp[(2) - (2)].term)); ;}
    break;

  case 58:
#line 371 "parser.y"
    {
  iodebug(DBG_PARSER2, "value: INTEGER %d\n", (yyvsp[(1) - (1)].intv));
  (yyval.term) = create_int_term((yyvsp[(1) - (1)].intv));
;}
    break;

  case 59:
#line 376 "parser.y"
    {
  iodebug(DBG_PARSER2, "value: DOUBLE %f\n", (yyvsp[(1) - (1)].doublev));
  (yyval.term) = create_double_term((yyvsp[(1) - (1)].doublev));
;}
    break;

  case 60:
#line 381 "parser.y"
    {
  iodebug(DBG_PARSER2, "value: STRING '%s'\n", (yyvsp[(1) - (1)].stringv));
  (yyval.term) = create_string_term((yyvsp[(1) - (1)].stringv));
;}
    break;

  case 61:
#line 387 "parser.y"
    { 
  inside_measurement_block = True; 
  measurement_num++;
  just_entered_measurement_block = True;
  if( get_my_global_rank() == get_output_rank() && !syntax_check_only) {
    if (log_source) 
      print_source(source_buf.measurement_pos[measurement_num-1], source_buf.measurement_pos[measurement_num]);
    print_output("# begin result \"%s\"\n", (yyvsp[(3) - (3)].stringv));
    measurement_name = (yyvsp[(3) - (3)].stringv);
  }
;}
    break;

  case 62:
#line 399 "parser.y"
    { 
  double start_time = 0.0;

  inside_measurement_block = False; 
  if( ! syntax_check_only ) {
    if( get_my_global_rank() == get_output_rank() ) start_time = MPI_Wtime(); 
    execute_statement((yyvsp[(5) - (7)].statement));
    flush_output();
    clear_locals(); 
    if( get_my_global_rank() == get_output_rank() ) {
      print_output("# end result \"%s\"\n", (yyvsp[(3) - (7)].stringv));
      print_output("# duration = %4.2f sec\n\n", MPI_Wtime() - start_time);
    }
  } else {
    store_measurement_pos();	
  }
  
;}
    break;

  case 65:
#line 421 "parser.y"
    { if( !syntax_check_only ) execute_statement((yyvsp[(2) - (2)].statement)); ;}
    break;

  case 66:
#line 423 "parser.y"
    { if( !syntax_check_only ) execute_statement((yyvsp[(1) - (1)].statement)); ;}
    break;

  case 67:
#line 427 "parser.y"
    {
  struct symboltable_entry *se;
  
  se = search_iterator((yyvsp[(1) - (1)].identifier));
  if( se == NULL ) 
    error_with_abort(10, "unknown iterator '%s' in line %d\n", 
		     (yyvsp[(1) - (1)].identifier), current_line_number);
  (yyval.symboltable_entry) = se;
;}
    break;


/* Line 1267 of yacc.c.  */
#line 2145 "parser.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 438 "parser.y"






