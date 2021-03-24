#ifndef ast_node_hpp
#define ast_node_hpp

#include <string>
#include <iostream>
#include <map>

#include <vector>

class Node;

typedef const Node *NodePtr;

struct Context
{
    std::map<std::string, int> params;
    int offset;
};

typedef std::vector<Context *> Scope;

class Node
{
public:
    virtual ~Node() {}
    Node() {}
    virtual void Compile(std::ostream &dst, Context *local) = 0;
    virtual void Compile(std::ostream &dst) = 0;
};

class Root
    : public Node
{
public:
    std::vector<Node *> functions;
    Scope local;

    virtual ~Root() {}

    Root()
    {
        functions = {};
        local = {};
    }

    Root(Node *_function)
    {
        functions.push_back(_function);
        local.push_back(new Context);
        std::cout << "root Created \n\n";
    }

    void AddFunction(Node *_function)
    {
        functions.push_back(_function);
        local.push_back(new Context);
    };

    void Compile(std::ostream &dst)
    {
        for (uint32_t i = 0; i < functions.size(); i++)
        {
            functions.at(i)->Compile(dst, local.at(i));
        }
    }
    virtual void Compile(std::ostream &dst, Context *local) {}
};

#endif
