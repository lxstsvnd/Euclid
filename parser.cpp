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

Parser::Parser(std::vector<Token> word, Token Start, Token eps)
{
	for(std::vector<Token>::iterator iter = --word.end(); iter != --word.begin(); --iter){_word.push_back(*iter);}
	//_word.push_back(eps);
	//_stack.push_back(eps);
	_stack.push_back(Start);
}
Parser::~Parser(){;} //Доделать
bool Parser::parse()
{
	std::vector<Token>::iterator word_iter, stack_iter;
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
                       	std::string buffer = _stack[iter].get_name();
                       	for(int jter = 0; jter < buffer.size(); ++jter){printf("%c", buffer[jter]);}
                       	printf("\n");
               	}
               	printf("\n");

		//проверка на ошибки кончилась
		if(_word.empty() && _stack.empty()){return true;}
		word_iter = --_word.end();
		stack_iter = --_stack.end();
		if((*word_iter).get_name() == (*stack_iter).get_name())
		{
			_word.pop_back();
			_stack.pop_back();
			continue;
		}
		if((*stack_iter).get_rules().empty())
		{
			printf("SOSI HUY\n");
			return false;
		}
		std::vector<Token> buffer = (*stack_iter).check_connects(*word_iter);
		if(buffer.empty()){return false;}
		_stack.pop_back();
		for(std::vector<Token>::iterator iter = --buffer.end(); iter != --buffer.begin(); --iter){_stack.push_back(*iter);}
		buffer.clear();
	}
}

