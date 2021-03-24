#ifndef ast_node_hpp
#define ast_node_hpp

#include <string>
#include <iostream>
#include <map>

#include <vector>

class Node;

typedef const Node *NodePtr;

class Node
{
public:
    virtual ~Node()
    {
    }

    //! Tell Codegen what node this code is
    virtual std::string getType() const = 0;
};

class Root : public Node
{
private:
    mutable std::vector<const Node *> functions;

public:
    virtual ~Root()
    {
        functions.clear();
    }
    Root()
    {
        functions = {};
    }

    Root(const Node *_function)
    {
        functions.push_back(_function);
    }

    void Addfunction(const Node *function)
    {
        functions.push_back(function);
    }

    std::string getType() const override
    {
        return "Root";
    }
};

#endif
