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

class Analyzer_token //токен анализатора
{
	private:
		std::string _nonterm; //нетерминальный символ
		std::vector<Analyzer_token> _rule; //раскрытие символа
		std::vector<Token> _connects; //связи с лексемами
	public:
		Analyzer_token();
		~Analyzer_token();
		void set_name(std::string name);
		void set_rule(std::vector<Analyzer_token> rule);
		void set_connects(std::vector<Token> connects);
		std::vector<Analyzer_token> get_rule();
		std::vector<Token> get_connects();
};

class Parser //парсер
{
	private:
		std::vector<Token> _word;
	public:
		Parser(std::vector<Token> word);
		~Parser();
};
