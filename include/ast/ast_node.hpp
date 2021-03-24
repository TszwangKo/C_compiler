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
    mutable std::vector<const Node *> functions;

public:
    virtual ~Node()
    {
        functions.clear();
    }
    Node()
    {
        functions = {};
    }

    Node(const Node *_function)
    {
        functions.push_back(_function);
    }

    void Compile(std::ostream &dst) {}

    void Addfunction(const Node *function)
    {
        functions.push_back(function);
    }
    //! tells Codegen what this node is
    virtual std::string getType() const
    {
        return "Root";
    }
};

#endif
