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
    std::string id;

    virtual ~Function()
    {
        delete CompoundStatement;
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        local->offset = 0;
        local->assign = true;
        local->mode = assign_type::function;
        name->Compile(dst, local);
        CompoundStatement->Compile(dst, local);
    }
    Function(std::string _type, Node *_name, Node *_CompoundStatement)
        : type(_type), name(_name), CompoundStatement(_CompoundStatement)
    {
        id = name->getName();
    }
};

#endif
