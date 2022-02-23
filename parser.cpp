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

Analyzer_token::Analyzer_token(){;}
Analyzer_token::~Analyzer_token(){;} //Доделать
void Analyzer_token::set_name(std::string name){_nonterm = name;}
void Analyzer_token::set_rule(std::vector<Analyzer_token> rule)
{
	for(std::vector<Analyzer_token>::iterator iter = rule.begin(); iter != rule.end(); ++iter){_rule.push_back(*iter);}
}
void Analyzer_token::set_connects(std::vector<Token> connects)
{
	for(std::vector<Token>::iterator iter = connects.begin(); iter != connects.end(); ++iter){_connects.push_back(*iter);}
}

Parser::Parser(std::vector<Token> word)
{
	for(std::vector<Token>::iterator iter = word.begin(); iter != word.end(); ++iter){_word.push_back(*iter);}
}
Parser::~Parser(){;} //Доделать

