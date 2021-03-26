#ifndef ast_statement_hpp
#define ast_statement_hpp

#include "ast_node.hpp"
#include "ast_expression.hpp"
#include <string>
#include <vector>
#include <iostream>

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
        int returnstacksize = getCount() * 4 + 16;
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
        std::string label2 = "I" + makeLC();
        std::string label3 = "I" + makeLC();
        expr->Compile(dst, local);
        dst << "move $t7, $v0" << std::endl;
        dst << "beq $t7, $zero, " << label2 << std::endl;
        statementif->Compile(dst, local);
        if (statementelse != NULL)
        {
            dst << "j   " << label3 << std::endl;
        }
        dst << label2 << ":" << std::endl;
        if (statementelse != NULL)
        {
            statementelse->Compile(dst, local);
            dst << label3 << ":" << std::endl;
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
        : expr(_expr), stat(_stat)
    {
        std::cout << "Check" << std::endl;
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        std::string label2 = "W" + makeLC();
        std::string label3 = "W" + makeLC();
        dst << "j   " << label2 << std::endl;
        dst << label3 << ":" << std::endl;
        stat->Compile(dst, local);
        dst << label2 << ":" << std::endl;
        expr->Compile(dst, local);
        dst << "bne $v0, $zero, " << label3 << std::endl;
        dst << "nop" << std::endl;
    }
};

class ForLoopStatement
    : public Root
{
private:
    Node *exps1;
    Node *exps2;
    Expression *expr;
    Node *stat;

public:
    virtual ~ForLoopStatement()
    {
        delete exps1;
        delete exps2;
        delete expr;
        delete stat;
    }

    ForLoopStatement()
        : expr(NULL), stat(NULL) {}

    ForLoopStatement(Node *_exps1, Node *_exps2, Expression *_expr, Node *_stat)
        : exps1(_exps1), exps2(_exps2), expr(_expr), stat(_stat)
    {
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        std::string label2 = "F" + makeLC();
        std::string label3 = "F" + makeLC();
        exps1->Compile(dst, local);
        dst << "j   " << label2 << std::endl;
        dst << label3 << ":" << std::endl;
        stat->Compile(dst, local);
        expr->Compile(dst, local);
        dst << label2 << ":" << std::endl;
        exps2->Compile(dst, local);
        dst << "bne $v0, $zero, " << label3 << std::endl;
        dst << "nop" << std::endl;
    }
};

#endif