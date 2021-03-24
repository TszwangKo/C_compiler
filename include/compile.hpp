#include "ast.hpp"
#include <iostream>

void compile(NodePtr g_root)
{
    if (g_root->getType() == "root")
    {
        std::cout << "Root";
    }
}
