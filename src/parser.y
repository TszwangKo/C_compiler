%code requires{
  #include "ast.hpp"
  #include <iostream>
  #include <cassert>

  extern Node *g_root; // A way of getting the AST out
  
  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);
}

%union{
     Node* node;
	 StatementList* statement;
	 Expression* expression;
     std::string *string;
     double number;
}

%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN



%type<node> external_declaration
%type<node> function_definition

%type<node> compound_statement
%type<string> type_specifier direct_declarator 

%type<statement> statement_list 
%type<node> statement
%type<node> jump_statement
%type<expression> expression additive_expression multiplicative_expression unary_expression logical_or_expression logical_and_expression


%type<number> CONSTANT
%type<string> IDENTIFIER 
%type<string> INT VOID


%start external_declaration

%%


external_declaration
	: function_definition	{  g_root = new Root($1); $$ = g_root; }
	;

function_definition
	: type_specifier direct_declarator compound_statement  { $$ = new Function(*$1,*$2,$3); }   
	;

type_specifier
	: VOID	{ $$ = new std::string("void");}
	| INT   { $$ = new std::string("int");}
	;

direct_declarator
	: IDENTIFIER  
	| direct_declarator '(' ')' { $$ = $1; }
	;

statement
	: jump_statement 
	;


compound_statement
	: '{' '}'{ $$ = new CompoundStatement(); }
	| '{' statement_list '}' { $$ = new CompoundStatement($2); }
	;


statement_list
	: statement     { $$ = new StatementList($1); }
	| statement_list statement { $$->AddStatement($2); }
	;

jump_statement
	: RETURN expression ';' { $$ = new ReturnStatement($2); }
	;

expression
    : additive_expression { $$ = new Expression($1); }
	| multiplicative_expression { $$ = new Expression($1); }
	| logical_or_expression { $$ = new Expression($1); }
	;

multiplicative_expression
	: unary_expression
	| multiplicative_expression '*' unary_expression { $$ = new MulOperator($1, $3); }
	| multiplicative_expression '/' unary_expression { $$ = new DivOperator($1, $3); }
	;


additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression { $$ = new AddOperator($1,$3); }
	| additive_expression '-' multiplicative_expression { $$ = new SubOperator($1,$3); }
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression { $$ = new LogicOrOperator($1, $3); }
	;

logical_and_expression
	: unary_expression
	| logical_and_expression AND_OP unary_expression  { $$ = new LogicOrOperator($1, $3); }

unary_expression
	: CONSTANT { $$ = new Constant($1); }
	| '-' CONSTANT { $$ = new Constant(-$2); } 
	;

%%

Node *g_root;
 
Node *parseAST()
{
  yydebug = 1;
  g_root=0;
  std::cout << " Been here \n\n";
  yyparse();
  std::cout << "parsed" << std::endl;
  return g_root;
}

