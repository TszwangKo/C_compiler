#include <iostream>
#include "./../include/ast.hpp"
#include "./../include/compile.hpp"
#include <fstream>

int main()
{
	std::ostream &dst(std::cout);
	dst << "test_called \n\n";
	Node *root = parseAST();
	dst << "parsed \n\n ";
	compile(dst, root);

	// Node *expr = new Constant(10);
	// Node *last = new ReturnStatement(expr);
	// Node *cmpstat = new CompoundStatement(last);
	// Node *function = new Function("int", "f", cmpstat);
	// Node *ast = new Root(function);
}
