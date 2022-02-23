#include "lexer.hpp"
#include <cstdio>
#include <cstdlib>

Tokenizer::Tokenizer(){;}
Tokenizer::~Tokenizer(){_variables.clear();}
void Tokenizer::add_variable(std::string symbol){_variables.push_back(symbol);}
void Tokenizer::add_constant(std::string constant){_constants.push_back(constant);}
bool Tokenizer::check_special(std::string special)
{
	if(special == "rightarrow" || special == "not" || special == "forall" || special == "exists" 
			|| special == "geq") return true;
	return false;
}
bool Tokenizer::isother(char symbol)
{
	if(symbol == '(' || symbol == ')' || symbol == '+' || symbol == '*' || symbol == '='
			|| symbol == '>') return true;
	return false;
}
void Tokenizer::add_token(std::string token_name){_tokens.push_back(Token(token_name));}
void Tokenizer::make_tokens(std::string input)
{
	std::string buffer;
	for(std::string::iterator iter = input.begin(); iter != input.end(); ++iter)
	{
		if(isspace(*iter)) continue; //пробельные символы игнорируются
		if(*iter == '\\') //токенизация спецсимвола
		{
			buffer.clear();
			++iter;
			if(iter == input.end()) exit(1); //бекслеш в конце строки
			while(*iter != '\\')
			{
				if(isspace(*iter))
				{
					++iter;
					continue;
				}
				buffer.push_back(*iter);
				++iter;
				if(iter == input.end()) exit(1); //бекслеш в конце строки
			}
			if(!check_special(buffer)) exit(2); //неправильно введен спецсимвол
			add_token(buffer);
			continue;
		}
		if(islower(*iter))
		{
			buffer.clear();
			buffer.push_back(*iter);
			add_variable(buffer);
			add_token(buffer);
			continue;
		}
		if(isdigit(*iter))
		{
			buffer.clear();
			while(isdigit(*iter))
			{
				buffer.push_back(*iter);
				++iter;
			}
			--iter;
			add_token(buffer);
			add_constant(buffer);
			continue;
		}
		if(isother(*iter))
		{
			buffer.clear();
			buffer.push_back(*iter);
			add_token(buffer);
			continue;
		}
		exit(3); //wrong symbols
	}
}
std::vector<Token> Tokenizer::get_variables(){return _variables;}
std::vector<Token> Tokenizer::get_constants(){return _constants;}
std::vector<Token> Tokenizer::get_tokens(){return _tokens;}
