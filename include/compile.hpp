#include "ast.hpp"
#include <iostream>
#include <vector>

void compile(std::ostream &dst, Node *nd)
{
    nd->Compile(dst);
}
