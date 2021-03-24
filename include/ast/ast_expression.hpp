#ifndef ast_expressions_hpp
#define ast_expressions_hpp

#include "ast_node.hpp"

#include <cmath>

class Expression
    : public Node
{
private:
    NodePtr expr;

public:
    virtual ~Expression()
    {
        delete expr;
    }
    Expression(NodePtr _expr)
        : expr(_expr)
    {
    }
    Expression() {}

    const NodePtr getexpr()
    {
        return expr;
    }

    virtual std::string getType() const override
    {
        return "Expression";
    }
};

class Constant
    : public Expression
{
private:
    double value;

public:
    ~Constant() {}

    virtual std::string getType() const override
    {
        return "constant";
    }
    Constant(double _value)
        : value(_value) {}

    double getvalue()
    {
        return value;
    }
};

#endif
