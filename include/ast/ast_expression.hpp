#ifndef ast_expressions_hpp
#define ast_expressions_hpp

#include "ast_node.hpp"
#include "<pair>"
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

class AssignmentExpression
    : public Expression
{
private:
    Node *lexpr;
    Node *rexpr;

public:
    virtual ~AssignmentExpression()
    {
        delete lexpr;
        delete rexpr;
    }
    AssignmentExpression(Node *_lexpr, Node *_rexpr)
        : lexpr(_lexpr), rexpr(_rexpr) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        local->assign = true;
        rexpr->Compile(dst, local);
        lexpr->Compile(dst, local);
    }
};

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

    void Compile(std::ostream &dst, Context *local)
    {
        if (local->assign == true)
        {
            if (local->params.find(name) == local->params.end())
            {
                // declaration
                local->params.insert(std::pair<std::string, int>(name, local->offset));
                local->offset += 4;
                dst << "sw $2," << local->params[name] << "($sp)" << std::endl;
            }
            else
            {
                // assignemnt
                dst << "sw $2," << local->params[name] << "($sp)" << std::endl;
            }
            local->assign = false;
        }
        else if (local->assign == false)
        {
            if (!(local->params.find(name) == local->params.end()))
            {
                dst << "lw $2," << local->params(name) << "($sp)" << std::endl;
            }
            else
            {
                std::cerr << "variable" << name << "does not exist" << std::endl;
            }
        }
    }
};

#endif
