#include "parser.hpp"

Node::Node(){;}
Node::Node(Token* value){_value = value;}
Node::~Node(){;}
void Node::set_value(Token* value){_value = value;}
void Node::set_status(int stat){_nonterm = stat;}
Token* Node::get_value(){return _value;}
void Node::add_child(Node* child){_children.push_back(child);}

Parser::Parser(std::vector<Token> word, Token* Start)
{
	_root->set_value(Start);
	_root->set_status(1);
	_current.push_back(_root);
	for(std::vector<Token>::iterator iter = --word.end(); iter != --word.begin(); --iter){_word.push_back(*iter);}
	//_word.push_back(eps);
	//_stack.push_back(eps);
	_stack.push_back(Start);
}
Parser::~Parser(){;} //Доделать
bool Parser::parse()
{
	if(_word.empty()){return false;} //пустое слово отвергается по определению
	while(true)
	{
		if(_word.empty() && _stack.empty()){return true;}
		int word_size = _word.size()-1;
		int stack_size = _stack.size()-1;
		if(_word[word_size].get_name() == _stack[stack_size]->get_name()) //если терминалы на вершине стеков совпали
		{
			_word.pop_back();
			_stack.pop_back();
			continue;
		}
		if(_stack[stack_size]->get_name() == "$") //если нетерминал раскрывается в пустое слово, рассматривается отдельно
		{
			_stack.pop_back();
			continue;
		}
		if((_stack[stack_size]->get_rules()).empty()) return false; //если терминалы не совпали
		std::vector<Token*> buffer = _stack[stack_size]->check_connects(_word[word_size]);
		if(buffer.empty()){return false;} //если нет соответствующего правила вывода
		_stack.pop_back(); //иначе - раскрывается нетерминал
		for(int iter = buffer.size()-1; iter >=0; --iter){_stack.push_back(buffer[iter]);}
		reveal_current(buffer);
		buffer.clear();
	}
}

void Parser::reveal_current(std::vector<Token*> tokens)
{
	int current_size = _current.size()-1;
	Node* cur = _current[current_size];
	_current.pop_back();
	for(int iter = 0; iter < tokens.size(); ++iter)
	{
		Node* child = new Node(tokens[iter]);
		cur->add_child(child);
		if(tokens[iter]->check_name()) _current.push_back(child);
	}
}

