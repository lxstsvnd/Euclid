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

Parser::Parser(std::vector<Token> word, Token* Start)
{
	for(std::vector<Token>::iterator iter = --word.end(); iter != --word.begin(); --iter){_word.push_back(*iter);}
	//_word.push_back(eps);
	//_stack.push_back(eps);
	_stack.push_back(Start);
}
Parser::~Parser(){;} //Доделать
bool Parser::parse()
{
	if(_word.empty()){return false;}
	while(true)
	{
		for(int iter = 0; iter < _word.size(); ++iter)
                {
                       	std::string buffer = _word[iter].get_name();
                       	for(int jter = 0; jter < buffer.size(); ++jter){printf("%c", buffer[jter]);}
                       	printf("\n");
               	}
               	printf("\n");
               	for(int iter = 0; iter < _stack.size(); ++iter)
               	{
                       	std::string buffer = _stack[iter]->get_name();
                       	for(int jter = 0; jter < buffer.size(); ++jter){printf("%c", buffer[jter]);}
                       	printf("\n");
               	}
               	printf("\n");

		//проверка на ошибки кончилась
		if(_word.empty() && _stack.empty()){return true;}
		int word_size = _word.size()-1;
		int stack_size = _stack.size()-1;
		if(_word[word_size].get_name() == _stack[stack_size]->get_name())
		{
			_word.pop_back();
			_stack.pop_back();
			continue;
		}
		if(_stack[stack_size]->get_name() == "$")
		{
			_stack.pop_back();
			continue;
		}
		if((_stack[stack_size]->get_rules()).empty()) return false;
		std::vector<Token*> buffer = _stack[stack_size]->check_connects(_word[word_size]);
		if(buffer.empty()){return false;}
		_stack.pop_back();
		for(int iter = buffer.size()-1; iter >=0; --iter){_stack.push_back(buffer[iter]);}
		buffer.clear();
	}
}

