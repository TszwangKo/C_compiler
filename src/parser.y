%code requires{
  #include "ast.hpp"

  #include <cassert>

  extern const Node *g_root; // A way of getting the AST out
  StatementList* stmt_list = new StatementList();

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  extern void yyerror(const char *);
}

%union{
     Node* node;
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



%type<node> translation_unit external_declaration
%type<node> function_definition

%type<node> compound_statement
%type<string> type_specifier direct_declarator 

%type<statement> statement_list statement
%type<statement> jump_statement
%type<node> expression

%type<number> CONSTANT
%type<string> IDENTIFIER 
%type<string> INT VOID


%start translation_unit
%%

translation_unit
	: external_declaration  { $$ = g_root; $1 = $$; }
	| translation_unit external_declaration
	;

external_declaration
	: function_definition   { $$ = new Root($1); }
	;

function_definition
	: type_specifier direct_declarator compound_statement  { $$ = new Function(*$1,*$2,$3); }   
	;

type_specifier
	: VOID
	| INT   
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
	: statement     { stmt_list->AddStatement($1);}
	| statement_list statement { $$ = $$->AddStatement($2); }
	;

jump_statement
	: RETURN expression ';' { $$ = new ReturnStatement($2); }
	;

expression
    : CONSTANT { $$ = new Constant($1); } 
	;


%%

const Node *g_root;

extern const Node *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}

