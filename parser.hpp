#include <vector>
#include <string>
#include "token.hpp"

class Node //Вершина дерева разбора
{
	private:
		Token* _value;
		std::vector<Node*> _children; //указатели на потомков
		int _nonterm; //статус (1, если нетерминал)
	public:
		Node();
		Node(Token* value);
		~Node();
		void set_value(Token* value);
		void set_status(int stat);
		Token* get_value();
		void add_child(Node* child);
};

class Parser //парсер
{
	private:
		std::vector<Token> _word;
		std::vector<Token*> _stack;
		Node* _root;
		std::vector<Node*> _current;
		void reveal_current(std::vector<Token*> tokens);
	public:
		Parser(std::vector<Token> word, Token* Start);
		~Parser();
		bool parse();

};
