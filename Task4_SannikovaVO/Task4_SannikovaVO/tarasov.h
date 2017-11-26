#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <string>

template<typename T>
struct function_type
{
	using type = void;
};

template<typename Ret, typename Class, typename... Args>
struct function_type<Ret(Class::*)(Args...) const>
{
	using type = std::function<Ret(Args...)>;
};

template<typename F>
typename function_type<decltype(&F::operator())>::type
GetFunctionFromLambda(F const &func)
{
	return func;
}

template<bool>
struct VoidFunction {};


class Graph;

class flow_control {
private:
	bool active;
public:
	flow_control() {
		active = true;
	}
	void stop() {
		active = false;
	}
	operator bool() const {
		return active;
	}
};

class NodeBase	// Базовый класс для Node (не шаблонный)
{
public:
	virtual int getID() const = 0;
	virtual void addChildNode(NodeBase* base, int argNum) = 0;
	virtual void execute() = 0;
	virtual void addElement(void*, int argNum) = 0;
};

struct childNode
{
	NodeBase* base;
	int arg_num;		// номер аргумента, соответствующего данному звену относительно его родителя
};


template<typename Ret, typename... Args>
class Node : public NodeBase
{
private:
	std::function<Ret(Args...)> f;
	const Graph& graph;
	std::vector<void*> args;		// Вектор указателей на узлы предшествующие данному узлу
	std::vector<childNode> children;
	std::conditional_t<std::is_same<void, Ret>::value, int, Ret> p;
	int id, cur_args;

public:
	Node(int id, std::function<Ret(Args...)> f, const Graph& graph_) : graph(graph_)
	{
		this->id = id;
		this->f = f;
		args.resize(sizeof...(Args));
	}

	virtual int getID() const {
		return id;
	}
	virtual void addChildNode(NodeBase* base, int argNum) {
		children.push_back({ base, argNum });
	}
	virtual void execute() {
		execute(VoidFunction<std::is_same<void, Ret>::value>{});
	}

	virtual void addElement(void* ptr, int argNum) {
		args[argNum] = ptr;
		cur_args++;
	}

	/* Если функция в звене возвращает значение */
	void execute(VoidFunction<false>) {
		if (cur_args == args.size()) {
			p = callF();
			cur_args = 0;
			if (graph.fc) {
				for (childNode child : children) {	// После вызова (если у функции есть возвращаемое значение), вызываем узлы-дети;если void, то функция последняя
					child.base->addElement(static_cast<void*>(&p), child.arg_num);
					child.base->execute();
				}
			}
		}
	}

	/* Если функция в звене - void */
	void execute(VoidFunction<true>) {
		if (cur_args == args.size()) {
			callF();
			cur_args = 0;
		}
	}

	template<typename A>
	A getArg(int i) {
		return *static_cast<typename std::remove_reference<A>::type *>(args[i]);
	}

	Ret callF() {
		auto index = std::make_index_sequence<sizeof...(Args)>{};
		return callF_Ex(index);
	}

	template<std::size_t... S>
	Ret callF_Ex(std::index_sequence<S...>) {
		auto index = std::make_index_sequence<sizeof...(Args)>{};
		return f(getArg<Args>(S)...);
	}
};

class Graph
{
private:
	std::vector<std::unique_ptr<NodeBase>> nodes;	// узлы графа
	std::vector<std::vector<int>> links;			// Список смежности? В векторе links лежит вектор для каждого узла графа, в этих векторах - ID предшествующих звеньев

public:
	flow_control fc;
	template<typename... Args>
	NodeBase* createNode(Args... args) {	// Функция, вызываемая в main(), несколько параметров
		links.push_back(std::vector<int>());
		return createNodeEx(args...);
	}

	template <typename T, typename ...Args>	// Рекурсия - на каждом шаге из списка параметров выдергиваем очередное звено (пока не останется только функция)
	NodeBase* createNodeEx(T firstNode, Args... args) {
		links[links.size() - 1].push_back(firstNode->getID());
		return createNodeEx(args...);
	}

	template <typename F>
	NodeBase* createNodeEx(F func) {			// Когда остался только один параметр - лямбда функция
		return createLastNode(GetFunctionFromLambda(func));
	}

	template<typename Ret, typename... Args>
	NodeBase* createLastNode(std::function<Ret(Args...)> func) {
		int id = nodes.size();		// Вычисляем ID вершины
		NodeBase* new_node = new Node<Ret, Args...>(id, func, *this);
		int i = 0;
		for (int child : links[id]) {	// Цикл по всем предшественникам созданного только что узла
			nodes[child].get()->addChildNode(new_node, i++); // Ставим предшественникам только что созданный узел в качестве ребенка
		}
		std::unique_ptr<NodeBase> ptr(new_node);
		nodes.push_back(std::move(ptr));
		return nodes[id].get();
	}

	void run()
	{
		while (fc)
		{
			nodes[0].get()->addElement(static_cast<void*>(&fc), 0);
			nodes[0].get()->execute();
		}
	}
};
