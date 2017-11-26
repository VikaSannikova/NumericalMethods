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
FunctionFromLambda(F const &func)
{
	return func;
}

class flow_control {
private:
	bool working;
public:
	flow_control() {
		working = true;
	}
	void stop() {
		working = false;
	}
	operator bool() const {
		return working;
	}
};

template<bool>
struct Flag {};

class Graph;

class BaseNode
{
public:
	virtual int getID() const = 0;
	virtual void addChild(BaseNode* root, int argNum) = 0;
	virtual void run() = 0;
	virtual void addElement(void*, int argNum) = 0;
	virtual ~BaseNode() {};
};

struct childNode
{
	BaseNode* root;
	int nArg;
};

template<typename Ret, typename... Args>
class Node : public BaseNode
{
private:
	std::function<Ret(Args...)> f;
	const Graph& graph;
	std::vector<void*> args;		
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
	virtual void addChild(BaseNode* root, int argNum) {
		children.push_back({ root, argNum });
	}


	virtual void addElement(void* ptr, int argNum) {
		args[argNum] = ptr;
		cur_args++;
	}

	virtual void run() {
		running(Flag<std::is_same<void, Ret>::value>{});
	}

	void running(Flag<false>) {
		if (cur_args == args.size()) {
			p = callF();
			cur_args = 0;
			if (graph.fc) {
				for (childNode child : children) {
					child.root->addElement(static_cast<void*>(&p), child.nArg);
					child.root->run();
				}
			}
		}
	}

	void running(Flag<true>) {
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
		return callF_main(index);
	}

	template<std::size_t... S>
	Ret callF_main(std::index_sequence<S...>) {
		auto index = std::make_index_sequence<sizeof...(Args)>{};
		return f(getArg<Args>(S)...);
	}

	virtual ~Node() {
	}
};

class Graph
{
private:
	std::vector<std::unique_ptr<BaseNode>> nodes;	
	std::vector<std::vector<int>> links;		

public:
	flow_control fc;

	template<typename Ret, typename... Args>
	BaseNode* createLastNode(std::function<Ret(Args...)> func) {
		int id = nodes.size();		
		BaseNode* new_node = new Node<Ret, Args...>(id, func, *this);
		int i = 0;
		for (int child : links[id]) {	
			nodes[child].get()->addChild(new_node, i++); 
		}
		std::unique_ptr<BaseNode> ptr(new_node);
		nodes.push_back(std::move(ptr));
		return nodes[id].get();
	}

	template <typename T, typename ...Args>	
	BaseNode* creatingNode(T firstNode, Args... args) {
		links[links.size() - 1].push_back(firstNode->getID());
		return creatingNode(args...);
	}

	template <typename F>
	BaseNode* creatingNode(F func) {			
		return createLastNode(FunctionFromLambda(func));
	}

	template<typename... Args>
	BaseNode* createNode(Args... args) {	
		links.push_back(std::vector<int>());
		return creatingNode(args...);
	}
	
	void run()
	{
		while (fc)
		{
			nodes[0].get()->addElement(static_cast<void*>(&fc), 0);
			nodes[0].get()->run();
		}
	}
};
