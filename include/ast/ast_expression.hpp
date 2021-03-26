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
    virtual void notTrue() {}
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
    bool checknot;

public:
    ~Constant() {}

    Constant(double _value)
        : value(_value), checknot(false) {}

    void notTrue()
    {
        checknot = true;
    }

    void changeSign()
    {
        value = -value;
    }

    void Compile(std::ostream &dst, Context *local)
    {
        dst << "li $2, " << value << std::endl;
        if (checknot)
        {
            dst << "sltu $v0, $v0, 1" << std::endl;
            dst << "andi $v0, $v0, 0xff" << std::endl;
        }
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
                {
                    dst << "#" << getCount() << std::endl;
                    local->params.insert(std::pair<std::string, int>(name, local->offset));
                    local->offset += 4;
                    dst << "sw $2," << local->params[name] << "($sp)" << std::endl;
                    break;
                }
                case assign_type::initialise:
                {
                    dst << "#" << getCount() << std::endl;
                    local->params.insert(std::pair<std::string, int>(name, local->offset));
                    local->offset += 4;
                    break;
                }
                case assign_type::assign:
                {
                    dst << "### wrong assignment mode" << std::endl;
                    break;
                }
                case assign_type::function:
                {
                    int stacksize = -getCount() * 4 - 8;
                    int framePtrStore = -stacksize - 4;
                    //TODO: romove comment
                    dst << "#" << stacksize << std::endl;
                    dst << "#" << framePtrStore << std::endl;
                    //TODO: remove comment
                    dst << ".globl " << name << std::endl;
                    dst << "#-------fucntion def----------#" << std::endl;
                    dst << name << ":" << std::endl;
                    dst << "addiu $sp,$sp," << stacksize << std::endl;
                    dst << "sw $fp," << framePtrStore << "($sp)" << std::endl;
                    dst << "move $fp,$sp" << std::endl;
                    break;
                }
                case assign_type::param:
                {
                    dst << "#" << getCount() << std::endl;
                    local->params.insert(std::pair<std::string, int>(name, local->offset));
                    local->offset += 4;
                    dst << "sw $" << getCount() + 1 << "," << local->params[name] << "($sp)" << std::endl;
                }
                case assign_type::none:
                {
                    dst << "##############none, need instruction" << std::endl;
                    break;
                }
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

class PostFixExpression
    : public Expression
{
private:
    Node *expr;
    char op;

public:
    virtual ~PostFixExpression()
    {
        delete expr;
    }

    PostFixExpression(Node *_expr, char _op)
        : expr(_expr), op(_op) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        expr->Compile(dst, local);
        if (op == '+')
            dst << "addiu   $v0, $v0, 1" << std::endl; // Adds 1 if result there is "++" at the end of the expression
        if (op == '-')
            dst << "addiu   $v0, $v0, -1" << std::endl; // Subtracts 1 if result there os "--" at the end of the expression
    }
};

#endif
