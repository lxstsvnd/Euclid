#pragma once

#include <vector>
#include <string>
#include <deque>
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
		GREATER,

		//теперь объявляются нетерминальные символы//
		BLACKSPACE, //рабочий символ
		DNF, //ДНФ
		DNF_TMP, //вспомогательный нетерминал для ДНФ
		CONJ, //конъюнкция предикатов
		CONJ_TMP,
		PREDICATE, //предикат
		POLYNOM,
		ORDER,
		SUM,
		MULT,
		MULT_TMP, //вспомогательный нетерминал для умножения
		CONST,

		END,
		EPSILON
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
		"GREATER",

		"BLACKSPACE",
		"DNF",
		"DNF_TMP",
		"CONJ",
		"CONJ_TMP",
		"PREDICATE",
		"POLYNOM",
		"ORDER",
		"SUM",
		"MULT",
		"MULT_TMP",
		"CONST",

		"END",
		"EPSILON"
	};
	
	class Token
	{
		private:
		public:
			Token(enum TokenType type);
			Token();
			enum TokenType mType{WHITESPACE}; //тип токена
			std::string mText; //текст в токене
			bool mTerm = 1; //всегда терминал, т.к. во вводе

	};

	class Tokenizer
	{
		private:
			Token _cToken; //текущий токен
			std::vector<Token> _tokens; //токенизированный ввод
			void endToken(); //добавление токена в вектор после обработки
		public:
			Tokenizer(const std::string& Line); //токенизация ввода
			std::vector<Token> get_tokens(); //возврат токенизированного ввода
			void debugPrint();
	};
}
