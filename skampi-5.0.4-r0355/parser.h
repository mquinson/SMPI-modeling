/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1489 of yacc.c.  */
#line 135 "parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

