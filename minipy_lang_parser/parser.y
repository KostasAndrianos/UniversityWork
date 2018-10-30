/*
  Name: Kostas Andrianos
  Date: 29/1/2018
  Class: Compilers, Semester 5 (2017-2018), TEI of Athens

  README: This code is strictly analyzing the grammar of the language mini-python.
  As a result, there will be a trillion instances of nonsense instructions being accepted.
  The purpose of this code isn't to catch the errors but rather to make sure the code is
  structured as intended, in order for the semantic analyzer to make sense of it and identify
  the errors.
*/

%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE double
int errorflag=0;
extern char * yytext;
void yyerror(char  const* s);
extern FILE *yyin;
extern int yylineno;
extern int yylex();
int wrong_expr = 0;
int correct_expr = 0;
int correct_tokens = 0;

%}

/*
  Token declaration
*/
%error-verbose

%token ASSIGN_OP
%token PLUS_AS
%token MINUS_AS
%token MULTI_AS
%token DIV_AS
%token MOD_AS
%token FLOOR_AS
%token EXPON_AS
%token PLUS_OP
%token MINUS_OP
%token MULTI_OP
%token DIV_OP
%token MOD_OP
%token FLOOR_OP
%token EXPON_OP

%token TYPE
%token LEN
%token VARIABLE

%token NEWLINE
%token COMMENT
%token STRING
%token TOKEN_ERROR
%token STRING_NOT_CLOSED
%token STRING_CLOSING_WITH_WRONG_QUOTE_TYPE
%token FLOAT_WITH_NO_INT_OR_FRACTION
%token SCIENTIFIC_NOTATION_WITHOUT_OFFSET
%token NUMBER_STARTING_WITH_ZERO
%token VARIABLE_STARTING_WITH_NUMBER
%token STRING_STARTING_WITH_NUMBER
%token INT
%token FLOAT
%token INPUT
%token PRINT
%token SEP
%token END


%left '(' ')'
/*
  Start Conditions
*/

%start program


/*
  Rules
*/

%%
/*NaN
  This is a custom type that describes everything returning a Non Numerical object or being a NaN itself.
*/
NaN: STRING {correct_tokens+=1;}
	| VARIABLE {correct_tokens+=1;}
	| python_char_item_in_str_list_tuple | python_raw_string | python_list | python_tuple | python_input_method;

NUMBER: FLOAT | INT;

/*PRINTABLE
  This is a custom type that contains anything that can be printed.
*/
PRINTABLE: python_concat | python_type | python_len

/*ASSIGNABLE
  This is a custom type that contains anything that can be assigned to something else.
*/
ASSIGNABLE: python_concat | python_number_actions_concat | python_type | python_len

/*ABLE_TO_USE_AS_INDEX
  This is a custom type that contains anything that can be used as an idex (to reference an item in a list for example).
*/
ABLE_TO_USE_AS_INDEX: python_number_actions_concat | python_len | VARIABLE

/*OPERATOR
  This is a custom type that contains all operators used for concatenation or arithmetic operations
*/
OPERATOR: PLUS_OP | MINUS_OP | MULTI_OP | DIV_OP | MOD_OP | FLOOR_OP | EXPON_OP

/*OPERATOR_AS
  This is a custom type that contains all operators used for assignment.
*/
OPERATOR_AS: PLUS_AS | MINUS_AS | MULTI_AS | DIV_AS | MOD_AS | FLOOR_AS | EXPON_AS | ASSIGN_OP

NUM_METHOD_OP: python_len OPERATOR ABLE_TO_USE_AS_INDEX | ABLE_TO_USE_AS_INDEX {correct_tokens+=1;}

program:
	| program expr;

expr:	 NEWLINE
  | python_assignment NEWLINE {correct_expr++;printf("\n");}
  | COMMENT
  | python_print_method NEWLINE {correct_expr++;printf("\n");}
	| error NEWLINE {errorflag=1; yyerrok;};
/*python_type
  Syntax for calling a method that returns the type of the object given to it and can be either used in an assignment
  or a concatenation operation. The latter is wrong logically but there is no syntax restriction to prevent
  it from happening. python_type like any other method returning something, can't be used standalone.
*/
python_type:	TYPE '(' STRING ')' {correct_tokens+=2;}
  | TYPE '(' NUMBER ')' {correct_tokens+=2;}
  | TYPE '(' VARIABLE ')' {correct_tokens+=2;};

/*python_len
  Syntax for calling a method that returns the length of the object given to it and can be either used in an assignment
  or a concatenation operation. The latter is valid for arithmetic operations, but is invalid for anything else.
  python_len can't be used standalone
*/
python_len:	LEN '(' STRING ')' {correct_tokens+=2;}
  | LEN '(' VARIABLE ')' {correct_tokens+=2;}
  | LEN '(' python_list ')' {correct_tokens+=1;}
  | LEN '(' python_tuple ')' {correct_tokens+=1;};

/*python_char_item_in_str_list_tuple
  Using this syntax, the programmer can reference a character in a string
  and a list or tuple element. Indexing starts from 0
*/
python_char_item_in_str_list_tuple:	STRING '[' NUM_METHOD_OP ']' {correct_tokens+=2;}
  | VARIABLE '[' NUM_METHOD_OP ']' {correct_tokens+=2;}
  | VARIABLE '[' NUM_METHOD_OP ']''[' NUM_METHOD_OP ']' {correct_tokens+=3;};

/*python_assignment
  Using this syntax, the programmer can assign anything ASSIGNABLE
  to a VARIABLE or a python_char_item_in_str_list_tuple.
*/
python_assignment: VARIABLE OPERATOR_AS ASSIGNABLE {correct_tokens+=2;}
  | python_char_item_in_str_list_tuple OPERATOR_AS ASSIGNABLE {correct_tokens+=2;};

/*python_concat
  Using this syntax, the programmer can assign anything ASSIGNABLE
  to a VARIABLE or a python_char_item_in_str_list_tuple. Again, many restrictions apply
  logically but, syntax wise, it's identical for all NaN types.
*/
python_concat: NaN
  | python_concat PLUS_OP python_concat {correct_tokens+=1;};

/*python_raw_string
  Typing r in front of a STRING literal, will take the string as is instead of parsing
  special characters. (\n will appear in the string instead of changing a line)
*/
python_raw_string: 'r' STRING {correct_tokens+=2;};

/*python_number_actions_concat
  Syntax for assigning a sign to a NUMBER or making numerical operations.
  Using space between operators and operands is critical. Not doing so,
  will render the expression unparsable due to the ability of the language
  to use signed numbers, conflicting with the actual PLUS and MINUS operators
*/
python_number_actions_concat: VARIABLE {correct_tokens+=1;}
	| NUMBER {correct_tokens+=1;}
	| PLUS_OP NUMBER {correct_tokens+=2;}
	| MINUS_OP NUMBER {correct_tokens+=2;}
  | python_number_actions_concat OPERATOR python_number_actions_concat {correct_tokens+=1;};

/*python_input_method
  Syntax for the INPUT method which can present the user with a prompt
  or be used without one. The prompt can be a NaN or a concatenation of NaN
*/
python_input_method: INPUT '('  ')' {correct_tokens+=1;}
  | INPUT '(' python_concat ')'{correct_tokens+=1;};

/*python_print_method
  Syntax for the PRINT method which prints it's input. The input
  can be nothing at all (which changes a line) or something PRINTABLE.
  Using sep as an argument, will define a separator, which can only be a NaN.
  Using end as an argument, will define the ending character or string,
  which can only be a NaN. Default is \n.
*/
python_print_method: PRINT '('  ')' {correct_tokens+=1;}
  | PRINT '(' print_part ')' {correct_tokens+=1;};

print_part: STRING ',' print_part {correct_tokens+=1;}
	| PRINTABLE ',' print_part
	| python_number_actions_concat ',' print_part
	| SEP ASSIGN_OP NaN ',' END ASSIGN_OP NaN {correct_tokens+=4;}
	| PRINTABLE
	| python_number_actions_concat
	| SEP ASSIGN_OP NaN {correct_tokens+=2;}
	| END ASSIGN_OP NaN {correct_tokens+=2;};
/*python_list
  Syntax for defining a list. This can't be used standalone
*/
python_list: '[' comma_separated_tokens ']';

/*python_tuple
  Syntax for defining a tuple. This can't be used standalone
*/
python_tuple: '(' comma_separated_tokens ')';

/*comma_separated_tokens
  The structure of the items in a python_list or a python_tuple
*/
comma_separated_tokens: ASSIGNABLE {correct_tokens+=1;}
  | comma_separated_tokens ',' comma_separated_tokens;
%%

void yyerror (const char* msg) {
	wrong_expr++;
	printf("Line: %d FAILURE %s\n", yylineno,msg);
}

int main (int argc, char **argv) {
	int i;
	if (argc == 2)
		yyin = fopen(argv[1],"r");
	else
		yyin=stdin;

  if (yyparse() == 0 && errorflag == 0)
     fprintf(stderr, "Successful parsing.\n");
  else
     fprintf(stderr, "error(s) found!\n");
 	printf("\n#CORRECT TOKENS %d", correct_tokens);
	printf("\n#WRONG EXPRESSIONS %d", wrong_expr);
	printf("\n#CORRECT EXPRESSIONS %d", correct_expr);
  return 0;
}
