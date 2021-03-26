#ifndef ast_statement_hpp
#define ast_statement_hpp

#include "ast_node.hpp"
#include "ast_expression.hpp"
#include <string>
#include <vector>

class CompoundStatement
    : public Root
{
private:
    Node *stat_list;
    Node *declar;

public:
    virtual ~CompoundStatement()
    {
        delete stat_list;
    }
    CompoundStatement()
        : stat_list(NULL) {}

    CompoundStatement(Node *_stat_list)
        : stat_list(_stat_list), declar(NULL) {}

    CompoundStatement(Node *_declar, Node *_stat_list)
        : stat_list(_stat_list), declar(_declar) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#--------CompoundStatement----------#" << std::endl;
        if (declar != NULL)
        {
            declar->Compile(dst, local);
        }
        if (stat_list != NULL)
        {
            stat_list->Compile(dst, local);
        }
    }
};

class StatementList
    : public Root
{
private:
    std::vector<Node *> stat;

public:
    virtual ~StatementList() {}

    StatementList()
    {
        stat = {};
    }

    StatementList(Node *_stat)
    {
        stat.clear();
        stat.push_back(_stat);
    }

    StatementList *AddStatement(Node *_stat)
    {
        stat.push_back(_stat);
        return this;
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "#-----Statement_list-----#" << std::endl;
        for (uint32_t i = 0; i < stat.size(); i++)
        {
            stat.at(i)->Compile(dst, local);
        }
    }
};

class ReturnStatement
    : public Root
{
private:
    Node *expr;

public:
    virtual ~ReturnStatement()
    {
        delete expr;
    }

    ReturnStatement()
        : expr(NULL) {}

    ReturnStatement(Node *_expr)
        : expr(_expr) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        expr->Compile(dst, local);
        int returnstacksize = getCount() * 4 + 8;
        int framePtrStore = returnstacksize - 4;
        dst << "move $sp,$fp" << std::endl;
        dst << "lw $fp," << framePtrStore << "($sp)" << std::endl;
        dst << "addiu $sp,$sp," << returnstacksize << std::endl;
        dst << "j $31" << std::endl;
        dst << "nop" << std::endl;
    }
};

class SelectionStatement
    : public Root
{
private:
    Expression *expr;
    Node *statementif;
    Node *statementelse;

public:
    virtual ~SelectionStatement()
    {
        delete expr;
        delete statementif;
        delete statementelse;
    }

    SelectionStatement()
        : expr(NULL), statementif(NULL), statementelse(NULL) {}

    SelectionStatement(Expression *_expr, Node *_statementif)
        : expr(_expr), statementif(_statementif), statementelse(NULL) {}

    SelectionStatement(Expression *_expr, Node *_statementif, Node *_statementelse)
        : expr(_expr), statementif(_statementif), statementelse(_statementelse) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        expr->Compile(dst, local);
        dst << "move $t7, $v0" << std::endl;
        dst << "beq $t7, $zero, I2" << std::endl;
        statementif->Compile(dst, local);
        if (statementelse != NULL)
        {
            dst << "j   I3" << std::endl;
        }
        dst << "I2:" << std::endl;
        if (statementelse != NULL)
        {
            statementelse->Compile(dst, local);
            dst << "I3:" << std::endl;
        }
    }
};

class WhileLoopStatement
    : public Root
{
private:
    Expression *expr;
    Node *stat;


public:
    virtual ~WhileLoopStatement()
    {
        delete expr;
        delete stat;
    }

    WhileLoopStatement()
        : expr(NULL), stat(NULL) {}

    WhileLoopStatement(Expression *_expr, Node *_stat)
        : expr(_expr), stat(_stat) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        dst << "j   W2" << std::endl;
        dst << "W3:" << std::endl;
        stat->Compile(dst, local);
        dst << "W2:" << std::endl;
        expr->Compile(dst, local);
        dst << "bne $v0, $zero, W3" << std::endl;
        dst << "nop" << std::endl;
    }
};

#endif