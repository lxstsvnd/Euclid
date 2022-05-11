#pragma once

#include "lexer.hpp"
#include "formulas_server.hpp"
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
			Kirill::DNF _dnf; //ДНФ, собранная из дерева
			Kirill::Conjunct _conjunct;
			Kirill::Greater_predicate* _grPredicate;
			Kirill::Equality_predicate* _eqPredicate;
			std::vector<std::pair<mpz_class, int>> _leftPolynomCoefs;
			std::vector<std::pair<mpz_class, int>> _rightPolynomCoefs;
			std::vector<std::pair<mpz_class, int>> _resultCoefsTMP;
			std::vector<mpz_class> _resultCoefs;

			bool _negFlag{false};
			int _degree{0};
			mpz_class _curCoefficient{1};
			bool _leftFlag{false}; //если левый многочлен записан

			void _merge(std::vector<std::pair<mpz_class, int>> a, std::vector<std::pair<mpz_class, int>> b);

			void _getUpperNode(const Node* toNode); //поиск верхнего в стеке нетерминала
			void _addNode(const Node* addedNode); //замена раскрытого нетерминала
			const int _ifMatched(Node* nTerm, Token Term); //анализ LL-таблицы

			void _makeDNF(const Node* fromNode);
			void _makeConjunct(const Node* fromNode);
			void _makePredicate(const Node* fromNode);

			void _deleteFromNode(const Node* fromNode);
		public:
			parseTree(const std::vector<Token>& Line);
			~parseTree();
			void getUpperNode(); //обновление текущего вызываемого нетерминала
			void parse(); //парсинг вектора токенов
			void makeDNF();
			Kirill::DNF getDNF();
	};
}
