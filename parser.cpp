#include "parser.hpp"

Node::Node(std::string value){_value = value;}
Node::~Node()
{
	_value.clear();
	for(std::vector<Node*>::iterator iter = _children.begin(); iter != _children.end(); ++iter){*iter = NULL;}
	_children.clear();
}
void Node::add_child(Node* child_value){_children.push_back(child_value);}
Node* Node::get_child(int child_position){return _children[child_position];}

Parser::Parser(std::vector<Token> word)
{
	for(std::vector<Token>::iterator iter = word.begin(); iter != word.end(); ++iter){_word.push_back(*iter);}
}
Parser::~Parser(){;} //Доделать

