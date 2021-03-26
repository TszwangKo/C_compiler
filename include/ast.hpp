#ifndef ast_hpp
#define ast_hpp

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "ast/ast_node.hpp"
#include "ast/ast_expression.hpp"
#include "ast/ast_declarator.hpp"
// #include "ast/ast_primitives.hpp"
#include "ast/ast_operators.hpp"
// #include "ast/ast_unary.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_function.hpp"

extern Node *parseAST();

// /* ======================    Defining Node    ======================= */
// class Node;
// class List;

// typedef const Node *NodePtr;

// extern const Node *parseAST();

// class Node
// {

// public:

//     virtual ~Node()
//     {
//     }

//     void Addfunction(const Node * _Function) const
//     {
//         functions.push_back(_Function);
//     }

//     virtual std::string getNodeType()  const
//     {
//         return "root";
//     }
//     mutable std::vector<const Node *> functions;
//     virtual std::string getType() const {
//     }
// };

// /* ======================  Defining Function  ======================= */
// class Function : public Node
// {
//     public:
//     std::string type;
//     std::string id;
//     NodePtr CompoundStatement;
//     //NodePtr params;

//     Function(std::string _type, std::string _id, const NodePtr _CompoundStatement )
//         : type(_type), id(_id), CompoundStatement(_CompoundStatement)
//     {
//     }
//     virtual ~Function(){}

//     std::string getType() const override
//     {
//         return "Function";
//     }
// };

// class List: public Node{
//     protected:
//     mutable std::vector<const Node *> children;

//     public:
//     List(std::vector<const Node *> _children): children(_children){}

//     ~List();

//     //* Add a child;
//     const Node* add(const Node *child) const{}
//     std::string getType() const override
//     {
//         return "List";
//     }
// };

// class Program: public List {
//     public:
//     Program(std::vector<const Node *> _children) : List(_children){
//     }
// };

// /* ======================  Defining Expression  ======================= */

// class Expression : public Node
// {
// public:
//     virtual void print(std::ostream &dst) const {}
// };

// class Constant : public Expression
// {
//     int value;
//     Constant(int _val) : value(_val) {}
//     virtual std::string getType() const override
//     {
//         return "Constant";
//     }
// };

// class Identifier : public Expression
// {
//     std::string id;
//     Identifier(std::string _id): id(_id){};
//     virtual std::string getType() const override
//     {
//         return "CONSTANT";
//     }
// };

// /* ======================  Defining Statements  ======================= */
// class Statement : public Node
// {
//     std::string type;
//     virtual void print() {}
// };

// class CompoundStatement : public Statement
// {
//     protected:
//     const List *stats;
//     //constructors
//     public:
//     CompoundStatement() : stats (new List ({}) )
//     {
//     }
//     CompoundStatement(const List * _stats) : stats(_stats){

//     }
// };

// class ReturnStatement : public Statement
// {
//     protected:
//     int value;

//     public:
//     ReturnStatement(){
//         value = 0;
//     }

//     ReturnStatement(double _value)
//         : value(_value)
//     {
//     }
//     std::string getType()
//     {
//         return "ReturnStatement";
//     }

// };

#endif
