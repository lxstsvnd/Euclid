#pragma once

#include <vector>
#include <string>

class Token
{
	private:
		std::string _token_name;
	public:
		Token(std::string token_name);
		Token();
		~Token();
		std::string get_name();
		void set_name(std::string new_name);
};
