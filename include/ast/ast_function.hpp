#ifndef ast_functions_hpp
#define ast_functions_hpp

#include "ast_node.hpp"

#include <cmath>
#include <string>
#include <vector>

class Function
    : public Node
{
private:
    std::string type;
    std::string name;
    const NodePtr CompoundStatement;

public:
    virtual ~Function()
    {
        delete CompoundStatement;
    }

    Function(std::string _type, std::string _name, const NodePtr _CompoundStatement)
        : type(_type), name(_name), CompoundStatement(_CompoundStatement) {}

    std::string
    gettype() const
    {
        return type;
    }

    std::string getname() const
    {
        return name;
    }

    std::string getType() const override
    {
        return "Function";
    }
};

#endif
