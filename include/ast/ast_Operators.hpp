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
        dst << "#---Add---#" << std::endl;
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
        dst << "#---Sub---#" << std::endl;
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
        dst << "#---mult---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t1, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "mul $v0, $t1, $v0" << std::endl;
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
        dst << "#---divide---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t1, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "div $t1, $v0" << std::endl;
        dst << "mflo $v0" << std::endl;
    }
};

class ModuloOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "%";
    }

public:
    ModuloOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---modulo---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t1, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "div $t1, $v0" << std::endl;
        dst << "mfhi $v0" << std::endl;
    }
};

class InclusiveOrOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "|";
    }

public:
    InclusiveOrOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---bitwise_or---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "or $v0, $t2, $v0" << std::endl;
    }
};

class ExclusiveOrOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "^";
    }

public:
    ExclusiveOrOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---bitwise_xor---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "xor $v0, $t2, $v0" << std::endl;
    }
};

class AndOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "&";
    }

public:
    AndOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---bitwise_and---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "and $v0, $t2, $v0" << std::endl;
    }
};

class LogicEqOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "==";
    }

public:
    LogicEqOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "xor $v0, $t2, $v0" << std::endl;
        dst << "sltiu  $v0, $v0, 1" << std::endl;
        dst << "andi $v0, $v0, 0x00ff" << std::endl;
    }
};

class LogicNeqOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "!=";
    }

public:
    LogicNeqOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "xor $v0, $t2, $v0" << std::endl;
        dst << "sltu  $v0, $zero, $v0" << std::endl;
        dst << "andi $v0, $v0, 0x00ff" << std::endl;
    }
};

class LessThanOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "<";
    }

public:
    LessThanOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "slt  $v0, $t2, $v0" << std::endl;
        dst << "andi $v0, $v0, 0x00ff" << std::endl;
    }
};

class GreaterThanOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return ">";
    }

public:
    GreaterThanOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "slt  $v0, $v0, $t2" << std::endl;
        dst << "andi $v0, $v0, 0x00ff" << std::endl;
    }
};

class LessThanEqualOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "<=";
    }

public:
    LessThanEqualOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "slt  $v0, $t2, $v0" << std::endl;
        dst << "xori $v0, $v0, 0x1" << std::endl;
        dst << "andi $v0, $v0, 0x00ff" << std::endl;
    }
};

class GreaterThanEqualOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return ">=";
    }

public:
    GreaterThanEqualOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "slt  $v0, $v0, $t2" << std::endl;
        dst << "xori $v0, $v0, 0x1" << std::endl;
        dst << "andi $v0, $v0, 0x00ff" << std::endl;
    }
};

class ShiftLeftOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "<<";
    }

public:
    ShiftLeftOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "sll  $v0, $t2, $v0" << std::endl;
    }
};

class ShiftRightOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "<<";
    }

public:
    ShiftRightOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "sra  $v0, $t2, $v0" << std::endl;
    }
};

class LogicAndOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "&&";
    }

public:
    LogicAndOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        std::string label2 = "A" + makeLC();
        std::string label3 = "A" + makeLC();
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "beq  $t2, $zero, " << label2 << std::endl;
        dst << "nop" << std::endl;
        dst << "beq  $v0, $zero, " << label2 << std::endl;
        dst << "nop" << std::endl;
        dst << "li   $v0, 1" << std::endl;
        dst << "j    " << label3 << std::endl;
        dst << "nop" << std::endl;
        dst << label2 << ": " << std::endl;
        dst << "move $v0, $zero" << std::endl;
        dst << label3 << ": " << std::endl;
    }
};

class LogicOrOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "||";
    }

public:
    LogicOrOperator(Expression *_left, Expression *_right)
        : Operator(_left, _right)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        std::string label2 = "O" + makeLC();
        std::string label3 = "O" + makeLC();
        std::string label4 = "O" + makeLC();
        dst << "#---logical_equal---#" << std::endl;
        getLeft()->Compile(dst, local);
        dst << "move $t2, $v0" << std::endl;
        getRight()->Compile(dst, local);
        dst << "bne  $t2, $zero, " << label2 << std::endl;
        dst << "nop" << std::endl;
        dst << "beq  $v0, $zero, " << label3 << std::endl;
        dst << "nop" << std::endl;
        dst << label2 << ":" << std::endl;
        dst << "li   $v0, 1" << std::endl;
        dst << "j    " << label4 << std::endl;
        dst << "nop" << std::endl;
        dst << label3 << ":" << std::endl;
        dst << "move $v0, $zero" << std::endl;
        dst << label4 << ":" << std::endl;
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