#ifndef ast_statement_hpp
#define ast_statement_hpp

#include "ast_node.hpp"
#include <string>
#include <vector>

class Statement
    : public Node
{
public:
    virtual ~Statement() {}

    virtual std::string getType() const
    {
        return "Statement";
    }
};

class CompoundStatement
    : public Statement
{
private:
    const NodePtr stat_list;

public:
    virtual ~CompoundStatement()
    {
        delete stat_list;
    }
    CompoundStatement()
        : stat_list(NULL) {}

    CompoundStatement(const Node *_stat_list)
        : stat_list(_stat_list) {}

    virtual std::string getType() const
    {
        return "CompoundStatement";
    }
};

class StatementList
    : public Statement
{
private:
    mutable std::vector<const Node *> stat;

public:
    virtual ~StatementList() {}

    StatementList()
    {
        stat = {};
    }

    StatementList(const NodePtr _stat)
    {
        stat.clear();
        stat.push_back(_stat);
    }

    void AddStatement(const NodePtr _stat)
    {
        stat.push_back(_stat);
    }

    virtual std::string getType() const
    {
        return "StatementList";
    }
};

class ReturnStatement
    : public Statement
{
private:
    NodePtr expr;

public:
    virtual ~ReturnStatement()
    {
        delete expr;
    }

    ReturnStatement()
        : expr(NULL) {}

    ReturnStatement(NodePtr _expr)
        : expr(_expr) {}

    std::string getType() const override
    {
        return "ReturnStatement";
    }
};

#endif