#ifndef ast_operators_hpp
#define ast_operators_hpp

#include "ast_node.hpp"
#include "ast_expression.hpp"

class Operator
    : public Expression
{
private:
    Expression *left;
    Expression *right;

protected:
    Operator(Expression *_left, Expression *_right)
        : left(_left), right(_right)
    {
    }

public:
    virtual ~Operator()
    {
        delete left;
        delete right;
    }

    virtual const char *getOpcode() const = 0;

    Expression *getLeft() const
    {
        return left;
    }

    Expression *getRight() const
    {
        return right;
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---Operator---#" << std::endl;
    }
};

class AddOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "+";
    }

public:
    AddOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        getLeft()->Compile(dst, local);
        dst << "move $t0, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "addu $v0, $t0, $v0" << std::endl;
    }
};

class SubOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "-";
    }

public:
    SubOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        getLeft()->Compile(dst, local);
        dst << "move $t0, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "subu $v0, $t0, $v0" << std::endl;
    }
};

class MulOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "*";
    }

public:
    MulOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        getLeft()->Compile(dst, local);
        dst << "move $t0, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "mul $v0, $t0, $v0" << std::endl;
    }
};

class DivOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "/";
    }

public:
    DivOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        getLeft()->Compile(dst, local);
        dst << "move $t0, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "div $t0, $v0" << std::endl;
        dst << "mflo $v0" << std::endl;
    }
};

// class ExpOperator
//     : public Operator
// {
// protected:
//     virtual const char *getOpcode() const override
//     {
//         return "^";
//     }

// public:
//     ExpOperator(Expression *_left, Expression *_right)
//         : Operator(_left, _right)
//     {
//     }

//     virtual double evaluate(
//         const std::map<std::string, double> &bindings) const override
//     {
//         double vl = getLeft()->evaluate(bindings);
//         double vr = getRight()->evaluate(bindings);
//         return pow(vl, vr);
//     }
// };

#endif