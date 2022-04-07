#pragma once

#include "lexer.hpp"

namespace dnf_parser
{
	class Node
	{
		private:
		public:
			enum TokenType mType{WHITESPACE}; //тип вершины дерева
			std::string mText; //текст в токене (мб бесполезно)
			bool mTerm; //1, если терминал
			Node(int isTerm, enum TokenType type);
			Node();
			std::vector<const Node*> _chPtr{};
			std::vector<const Node*> getChildren() const; //получить потомков вершины
			void addChild(const Node* addedNode); //добавить потомка
	};

	class parseTree
	{
		private:
			Node* _root;
			Node* _upperNode; //текущий раскрываемый нетерминал
			std::vector<Token> _Line; //разбираемая последовательность терминалов
			void _getUpperNode(const Node* toNode); //поиск верхнего в стеке нетерминала
			void _addNode(const Node* addedNode); //замена раскрытого нетерминала
			const int _ifMatched(Node* nTerm, Token Term); //анализ LL-таблицы
		public:
			parseTree(const std::vector<Token>& Line);
			void getUpperNode(); //обновление текущего вызываемого нетерминала
			void parse(); //парсинг вектора токенов
	};
}
