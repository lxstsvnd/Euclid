#include <vector>
#include <string>
#include "token.hpp"

class Node //Вершина дерева разбора
{
	private:
		std::string _value;
		std::vector<Node*> _children; //указатель на потомка
	public:
		Node(std::string value);
		~Node();
		void add_child(Node* child_value);
		Node* get_child(int child_position);

};

class Parsed_Tree //дерево разбора формулы
{
	private:
		std::vector<Node*> _tree;
	public:
};

class Parser //парсер
{
	private:
		std::vector<Token> _word;
		std::vector<Token*> _stack;
	public:
		Parser(std::vector<Token> word, Token* Start);
		~Parser();
		bool parse();
};
