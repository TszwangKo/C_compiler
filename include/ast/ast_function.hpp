#ifndef ast_functions_hpp
#define ast_functions_hpp

#include "ast_node.hpp"

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

class Function
    : public Root
{
public:
    std::string type;
    Node *name;
    Node *CompoundStatement;

    virtual ~Function()
    {
        delete CompoundStatement;
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        local->offset = 0;
        int stacksize = -getCount() * 4 - 8;
        int framePtrStore = -stacksize - 4;
        //TODO: romove comment
        dst << "#" << stacksize << std::endl;
        dst << "#" << framePtrStore << std::endl;
        //TODO: remove comment
        dst << ".globl " << name->getName() << std::endl;
        dst << "#-------fucntion def----------#" << std::endl;
        dst << name->getName() << ":" << std::endl;
        dst << "addiu $sp,$sp," << stacksize << std::endl;
        dst << "sw $fp," << framePtrStore << "($sp)" << std::endl;
        dst << "move $fp,$sp" << std::endl;
        CompoundStatement->Compile(dst, local);
    }
    Function(std::string _type, Node *_name, Node *_CompoundStatement)
        : type(_type), name(_name), CompoundStatement(_CompoundStatement) {}
};

#endif
