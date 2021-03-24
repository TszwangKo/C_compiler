#ifndef ast_expressions_hpp
#define ast_expressions_hpp

#include "ast_node.hpp"

#include <cmath>

class Expression
    : public Root
{
private:
    Node *expr;

public:
    virtual ~Expression()
    {
        delete expr;
    }
    Expression(Node *_expr)
        : expr(_expr)
    {
    }
    Expression() {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        expr->Compile(dst, local);
    }
};

class Constant
    : public Root
{
private:
    double value;

public:
    ~Constant() {}

    Constant(double _value)
        : value(_value) {}

    void Compile(std::ostream &dst, Context *local)
    {
        dst << "li $2, " << value << std::endl;
    }
};

#endif
