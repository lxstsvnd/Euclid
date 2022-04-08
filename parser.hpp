#pragma once

#include "lexer.hpp"
#include "formulas.hpp"
#include <utility>
#include <algorithm>
#include <string>
#include <cstdlib>

namespace Dima
{
	class Node
	{
		private:
			std::vector<const Node*> _chPtr{}; //потомки вершины
			enum TokenType _mType{WHITESPACE}; //тип вершины дерева
			bool _mTerm; //1, если терминал
			bool _vTerm{0}; //1, если терминал посещен
			std::string _mText{""};
		public:
			Node(int isTerm, enum TokenType type);
			Node(int isTerm, std::string text, enum TokenType type);
			Node();
			void setType(enum TokenType newType);
			void setText(std::string newText);
			void setMTerm(bool mTerm);
			void setVTerm(bool vTerm);
			enum TokenType getType() const;
			std::string getText() const;
			bool getMTerm() const;
			bool getVTerm() const;
			std::vector<const Node*> getChildren() const; //получить потомков вершины
			void addChild(const Node* addedNode); //добавить потомка
	};

	class parseTree
	{
		private:
			Node* _root; //корень дерева
			Node* _upperNode; //текущий раскрываемый нетерминал
			std::vector<Token> _Line; //разбираемая последовательность терминалов

			std::vector<Kirill::Conjunct> _conjuncts{}; //конъюнкты из дерева разбора
			Kirill::Polynom* _curLPolynom{NULL}; //текущий левый полином
			Kirill::Polynom* _curRPolynom{NULL}; //текущий правый полином
			bool _negotiated{false}; //по умолчанию нет отрицания
			int degree{0}; //по умолчанию степень нулевая
			std::vector<std::pair<mpz_class, int>> _coefficients; //вспомогательный вектор, чтобы хранить степени переменной

			void _getUpperNode(const Node* toNode); //поиск верхнего в стеке нетерминала
			void _addNode(const Node* addedNode); //замена раскрытого нетерминала
			const int _ifMatched(Node* nTerm, Token Term); //анализ LL-таблицы
			void _getConjuncts(const Node* fromNode);
			std::vector<Kirill::Predicate*> _makeConjunct(const Node* fromNode);
			Kirill::Predicate* _makePredicate(const Node* fromNode);
			void _makePolynom(Kirill::Polynom* toPolynom, const Node* fromNode);
			void _makePolynomSpecial(const Node* fromNode, int coef);
		public:
			parseTree(const std::vector<Token>& Line);
			void getUpperNode(); //обновление текущего вызываемого нетерминала
			void parse(); //парсинг вектора токенов
			void makeConjuncts(); //извлечение конъюнктов из дерева
			std::vector<Kirill::Conjunct> getConjuncts();
	};
}
