#ifndef ast_functions_hpp
#define ast_functions_hpp

#include "ast_node.hpp"

#include <cmath>
#include <string>
#include <vector>

class Function
    : public Root
{
public:
    std::string type;
    std::string name;
    Node *CompoundStatement;

    virtual ~Function()
    {
        delete CompoundStatement;
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << ".globl " << name << std::endl;
        dst << name << ":" << std::endl;
        dst << "addiu $sp,$sp,-32" << std::endl;
        dst << "sw $fp,28($sp)" << std::endl;
        dst << "move $fp,$sp" << std::endl;
        CompoundStatement->Compile(dst, local);
    }
    Function(std::string _type, std::string _name, Node *_CompoundStatement)
        : type(_type), name(_name), CompoundStatement(_CompoundStatement) {}
};

#endif
