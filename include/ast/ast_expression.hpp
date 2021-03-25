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

class PrimaryExpression
    : public Expression
{
private:
    Node *expr;

public:
    virtual ~PrimaryExpression()
    {
        delete expr;
    }
    PrimaryExpression(Node *_expr)
        : expr(_expr) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        expr->Compile(dst, local);
    }
};

// class AssignmentExpression
//     : public Expression
// {
// private:
//     Node *lexpr;
//     Node *rexpr;

// public:
//     virtual ~AssignmentExpression()
//     {
//         delete lexpr;
//         delete rexpr;
//     }
//     AssignmentExpression(Node *_lvalue, Node *_rvalue)
//         : lvalue(_lvalue), rvalue(_rvalue) {}

//     virtual void Compile(std::ostream &dst, Context *local) override
//     {
//         rvalue->Compile(dst, local);
//         lvalue->Compile(dst, local);
//     }
// };

class Constant
    : public Expression
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

class Variable
    : public Expression
{
private:
    std::string name;

public:
    virtual ~Variable() {}
    Variable(std::string _name)
        : name(_name) {}
};

#endif
