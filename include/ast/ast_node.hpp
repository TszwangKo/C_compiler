#ifndef ast_node_hpp
#define ast_node_hpp

#include <string>
#include <iostream>
#include <map>

#include <vector>

class Node;

typedef const Node *NodePtr;

enum assign_type
{
    none,
    construct,
    assign,
    initialise,
    function,
    param
};

struct Context
{
    std::map<std::string, int> params;
    int offset;
    bool assign;
    assign_type mode;
    bool neg;
};

typedef std::vector<Context *> Scope;

// Sets unique labels
static int label_counter = 0;

static std::string makeLC()
{
    label_counter++;
    return std::to_string(label_counter);
}

class Node
{
public:
    virtual ~Node() {}
    Node() {}
    virtual void Compile(std::ostream &dst, Context *local) = 0;
    virtual void Compile(std::ostream &dst) = 0;
    virtual std::string getName() { return "Node"; }
};
static unsigned *Count = new unsigned(0);
class Root
    : public Node
{
public:
    std::vector<Node *> functions;
    Scope local;
    unsigned *count;

    virtual ~Root() {}

    unsigned getCount()
    {
        return *count;
    }
    void IncrementCount()
    {
        (*count)++;
    }
    Root()
    {
        functions = {};
        local = {};
        count = Count;
    }

    Root(Node *_function)
    {
        functions.push_back(_function);
        local.push_back(new Context);
    }

    void AddFunction(Node *_function)
    {
        functions.push_back(_function);
        local.push_back(new Context);
    }

    virtual std::string getName() override
    {
        return "root";
    }

    void Compile(std::ostream &dst)
    {
        for (uint32_t i = 0; i < functions.size(); i++)
        {
            functions.at(i)->Compile(dst, local.at(i));
        }
    }
    virtual void Compile(std::ostream &dst, Context *local) {}
};
// Register Number	Conventional Name	    Usage
// $0	            $zero	                Hard-wired to 0
// $1	            $at	                    Reserved for pseudo-instructions
// $2 - $3	        $v0, $v1	            Return values from functions
// $4 - $7	        $a0 - $a3	            Arguments to functions - not preserved by subprograms
// $8 - $15	        $t0 - $t7	            Temporary data, not preserved by subprograms
// $16 - $23	    $s0 - $s7	            Saved registers, preserved by subprograms
// $24 - $25	    $t8 - $t9	            More temporary registers, not preserved by subprograms
// $26 - $27	    $k0 - $k1	            Reserved for kernel. Do not use.
// $28	            $gp	                    Global Area Pointer (base of global data segment)
// $29          	$sp	                    Stack Pointer
// $30	            $fp	                    Frame Pointer
// $31          	$ra	                    Return Address
// $f0 - $f3	    -	                    Floating point return values
// $f4 - $f10	    -	                    Temporary registers, not preserved by subprograms
// $f12 - $f14	    -	                    First two arguments to subprograms, not preserved by subprograms
// $f16 - $f18	    -	                    More temporary registers, not preserved by subprograms
// $f20 - $f31	    -	                    Saved registers, preserved by subprograms

#endif
