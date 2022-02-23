#pragma once

#include <vector>
#include <string>

class Token
{
	private:
		std::string _name;
		std::vector<std::vector<Token>> _rule;
		std::vector<std::vector<Token>> _connects;

	public:
		Token(std::string name);
		Token();
		~Token();
		std::string get_name();
		void set_name(std::string new_name);
		void set_rule(std::vector<Token> rule);
		void set_connects(std::vector<Token> connects);
};
