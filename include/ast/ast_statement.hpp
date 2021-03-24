#ifndef ast_statement_hpp
#define ast_statement_hpp

#include "ast_node.hpp"
#include <string>
#include <vector>

class CompoundStatement
    : public Root
{
private:
    Node *stat_list;

public:
    virtual ~CompoundStatement()
    {
        delete stat_list;
    }
    CompoundStatement()
        : stat_list(NULL) {}

    CompoundStatement(Node *_stat_list)
        : stat_list(_stat_list) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        stat_list->Compile(dst, local);
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

    void AddStatement(Node *_stat)
    {
        stat.push_back(_stat);
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        for (uint32_t i = 1; i < stat.size(); i++)
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
        dst << "li $2,10" << std::endl;
        dst << "move $sp,$fp" << std::endl;
        dst << "lw $fp,28($sp)" << std::endl;
        dst << "addiu $sp,$sp, 32" << std::endl;
        dst << "j $31" << std::endl;
        dst << "nop";
    }
};

#endif