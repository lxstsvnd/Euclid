#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

namespace dnf_parser
{
	enum TokenType //объявление токенов
	{
		WHITESPACE, //служебный токен (нет в токенизированной строке)
		IDENTIFIER, //переменная
		INTEGER_LITERAL, //целое число
		SPECIAL_SEQUENCE, //служебный (для записи всех спецсимволов)
		DISJOINT, //дизъюнкция
		CONJUNCT, //конъюнкция
		RESISTANCE, //отрицание
		POWER, //степень
		LBRAC, //скобки//
		RBRAC, //      //
		PLUS,
		MULTIPLY,
		EQUAL,
		GREATER
	};

	static const char* sTokenTypeStrings[] =
	{
		"WHITESPACE",
		"IDENTIFIER",
		"INTEGER_LITERAL",
		"SPECIAL_SEQUENCE",
		"DISJOINT",
		"CONJUNCT",
		"RESISTANCE",
		"POWER",
		"LBRAC",
		"RBRAC",
		"PLUS",
		"MULTIPLY",
		"EQUAL",
		"GREATER"
	};
	
	class Token
	{
		private:
		public:
			enum TokenType mType{WHITESPACE}; //тип токена
			std::string mText; //текст в токене

	};

	class Tokenizer
	{
		private:
			Token _cToken; //текущий токен
			std::vector<Token> _tokens; //токенизированный ввод
			void endToken(); //добавление токена в вектор после обработки
		public:
			Tokenizer(const std::string& Line); //токенизация ввода
			std::vector<Token> get_tokens() const; //возврат токенизированного ввода
			void debugPrint();
	};
}
