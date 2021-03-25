#ifndef ast_expressions_hpp
#define ast_expressions_hpp

#include "ast_node.hpp"
#include "ast_expression.hpp"
#include <cmath>
#include <vector>

class DeclarationList
    : public Root
{
private:
    std::vector<Node *> declar;

public:
    virtual ~DeclarationList() {}

    DeclarationList(Node *_declaration)
    {
        declar.clear();
        declar.push_back(_declaration);
    }

    void AddDaclaration(Node *_declaration)
    {
        declar.push_back(_declaration);
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        for (uint32_t i = 0; i < decalr.size(); i++)
        {
            declar.at(i)->Compile(dst, local);
        }
    }
};

class Declaration
    : public Root
{
private:
    std::string type;
    Node *init_declar_list;

public:
    virtual ~Declaration()
    {
        delete init_declar_list;
    }

    Declaration(std::string _type, Node *_init_declar_list)
        : type(_type), init_declar_list(_init_declar_list) {}

    Declaration(std::string _type)
        : type(_type), init_declar_list(NULL) {}

    Declaration(std::string _type, Node *_init_declar_list_list)
        : type(_type), init_declar_list(_init_declar_list) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        init_declar_list->Compile(dst, local);
    }
};

class InitDeclaratorList
    : public Root
{
private:
    std::vector<Node *> init_declar;

public:
    virtual ~InitDeclaratorList() {}

    InitDeclaratorList(Node *_init_declaration)
    {
        init_declar.clear();
        init_declar.push_back(_init_declaration);
    }

    void AddInitDeclarator(Node *_init_declaration)
    {
        init_declar.push_back(_init_declaration);
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        for (uint32_t i = 0; i < init_declar.size(); i++)
        {
            init_declar.at(i)->Compile(dst, local);
        }
    }
};

class InitDeclarator
    : public Root
{
public:
    Node *declarator;
    Node *expr;

private:
    virtual ~InitDeclarator()
    {
        delete declarator;
        delete expr;
    }
    InitDeclarator(Node *_declarator, Node *_expr)
        : declarator(_declarator), expr(_expr) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        expr->Compile(dst, local);
        dst << "move $3,$2" << std::endl;
        declarator->Compile(dst, local);
    }
};

#endif
