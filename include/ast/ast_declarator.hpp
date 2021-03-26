#ifndef ast_declarator_hpp
#define ast_declarator_hpp

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

    void AddDeclaration(Node *_declaration)
    {
        declar.push_back(_declaration);
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        for (uint32_t i = 0; i < declar.size(); i++)
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

class DirectDeclarator
    : public Root
{
private:
    Node *variable;
    Node *params_list;

public:
    virtual ~DirectDeclarator()
    {
        delete variable;
        delete params_list;
    }

    virtual std::string getName() override
    {
        return variable->getName();
    }

    DirectDeclarator(Node *_variable)
        : variable(_variable), params_list(NULL) {}

    DirectDeclarator(Node *_declarator, Node *_params_list)
        : variable(_declarator), params_list(_params_list) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        if (params_list != NULL)
        {
            variable->Compile(dst, local);
            local->assign = true;
            local->mode = assign_type::param;
            params_list->Compile(dst, local);
        }
        else if (params_list == NULL)
        {

            variable->Compile(dst, local);
        }
    }
};

class InitDeclarator
    : public Root
{
private:
    Node *declarator;
    Node *expr;

public:
    virtual ~InitDeclarator()
    {
        delete declarator;
        delete expr;
    }
    InitDeclarator(Node *_declarator)
        : declarator(_declarator), expr(NULL) {}

    InitDeclarator(Node *_declarator, Node *_expr)
        : declarator(_declarator), expr(_expr) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        if (expr != NULL)
        {
            expr->Compile(dst, local);
            // dst << "move $3,$2" << std::endl;
            local->mode = assign_type::construct;
            local->assign = true;
            declarator->Compile(dst, local);
        }
        else if (expr == NULL)
        {
            local->mode = assign_type::initialise;
            local->assign = true;
            declarator->Compile(dst, local);
        }
    }
};
class ParameterDeclarationList
    : public Root
{
private:
    std::vector<Node *> param_declar;

public:
    virtual ~ParameterDeclarationList() {}

    ParameterDeclarationList(Node *_init_declaration)
    {
        param_declar.clear();
        param_declar.push_back(_init_declaration);
    }

    void AddParamDeclaration(Node *_init_declaration)
    {
        param_declar.push_back(_init_declaration);
    }

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        for (uint32_t i = 0; i < param_declar.size(); i++)
        {
            local->param_index = i;
            param_declar.at(i)->Compile(dst, local);
        }
    }
};

class ParameterDeclaration
    : public Root
{
private:
    std::string type;
    Node *declarator;

public:
    virtual ~ParameterDeclaration()
    {
        delete declarator;
    }
    ParameterDeclaration(std::string _type, Node *_declarator)
        : type(_type), declarator(_declarator) {}

    virtual void Compile(std::ostream &dst, Context *local) override
    {
        local->assign = true;
        local->mode = assign_type::param;
        declarator->Compile(dst, local);
    }
};

#endif
