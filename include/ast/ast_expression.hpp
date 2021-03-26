#ifndef ast_expression_hpp
#define ast_expression_hpp

#include "ast_node.hpp"
#include <cmath>
#include <map>
#include <string>
#include <iostream>

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
    Expression() : expr(NULL) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        if (expr == NULL)
        {
            return;
        }
        else
        {
            expr->Compile(dst, local);
        }
    }
    virtual void changeSign() {}
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

    AssignmentExpression(Node *_lexpr)
        : lexpr(_lexpr), rexpr(NULL) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        if (rexpr != NULL)
        {
            rexpr->Compile(dst, local);
            local->assign = true;
            local->mode = assign_type::assign;
            lexpr->Compile(dst, local);
        }
        else if (rexpr == NULL)
        {
            lexpr->Compile(dst, local);
        }
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

    void changeSign()
    {
        value = -value;
    }

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
        : name(_name)
    {
        std::cout << "Variable added\n";
        IncrementCount();
        std::cout << getCount() << std::endl;
    }

    std::string getName()
    {
        return name;
    }

    void Compile(std::ostream &dst, Context *local)
    {
        if (local->assign == true)
        {
            if (local->params.find(name) == local->params.end())
            {
                // declaration
                switch (local->mode)
                {
                case assign_type::construct:
                    dst << "#" << getCount() << std::endl;
                    local->params.insert(std::pair<std::string, int>(name, local->offset));
                    local->offset += 4;
                    dst << "sw $2," << local->params[name] << "($sp)" << std::endl;
                    break;
                case assign_type::initialise:
                    dst << "#" << getCount() << std::endl;
                    local->params.insert(std::pair<std::string, int>(name, local->offset));
                    local->offset += 4;
                    break;
                case assign_type::assign:
                    dst << "### wrong assignment mode" << std::endl;
                    break;
                case assign_type::none:
                    dst << "##############none, need instruction" << std::endl;
                }
            }
            else
            {
                // assignemnt
                dst << "sw $2," << local->params[name] << "($sp)" << std::endl;
            }
            local->assign = false;
            local->mode = assign_type::none;
        }
        else if (local->assign == false)
        {
            if (!(local->params.find(name) == local->params.end()))
            {
                dst << "lw $2," << local->params[name] << "($sp)" << std::endl;
            }
            else
            {
                std::cerr << "variable" << name << "does not exist" << std::endl;
            }
        }
    }
};

#endif
