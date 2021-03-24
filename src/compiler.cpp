#include <iostream>
#include "ast.hpp"
#include "compile.hpp"
#include <fstream>

extern FILE *yyin;

int main(int argc, char *argv[])
{
	std::ostream &dst(std::cout);
	std::string input_file = argv[2];
	std::string output_file = argv[4];
	std::ofstream Output_File(output_file);

	if (input_file != "")
	{
		FILE *fh;
		if ((fh = fopen(input_file.c_str(), "r")))
		{
			yyin = fh;
		}
		else
		{
			std::cerr << "Error: input file \"" << input_file << "\" could not be opened." << std::endl;
			return 1;
		}
	}

	// Node *expr = new Constant(10);
	// Node *last = new ReturnStatement(expr);
	// Node *cmpstat = new CompoundStatement(last);
	// Node *function = new Function("int", "f", cmpstat);
	// Node *ast = new Root(function);

	Node *ast = parseAST();
	dst << "parsed \n\n ";
	compile(Output_File, ast);
}
