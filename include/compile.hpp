#include "ast.hpp"
#include <iostream>
#include <vector>
#
void compile(std::ostream &dst, const NodePtr nd)
{
    if (nd->getType() == "Root")
    {
        for (int i = 0; i < nd->functions.size(); i++)
        {
            compile(nd->functions.at(i));
        }
    }
    else if (nd->getType() == "Function")
    {
    }
}
