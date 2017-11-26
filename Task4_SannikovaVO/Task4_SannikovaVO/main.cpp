#include "code.h"
#include <iostream>
#include <sstream>

const int kMaxCount = 10;

void createGraph(Graph& g) {
	auto generator = g.createNode([](flow_control& fc) {
		static int i = 0;
		if (i < kMaxCount) {
			return i++;
		}
		else {
			fc.stop();
			return 0;
		}
	});

	auto processor1 = g.createNode(generator, [](const int& i) {
		return double(i*i);
	});

	auto processor2 = g.createNode(generator, [](const int& count) {
		std::stringstream ss;
		for (int i = 0; i < count; ++i) {
			ss << "Hello!";
		}
		return ss.str();
	});

	g.createNode(generator, processor1, processor2, [](const int& i, const double& sqr, const std::string& str) {
		std::cout << i << " " << sqr << " " << str << std::endl;
	});
}

int main() {
	Graph g;
	createGraph(g);
	g.run();
	int n;
	std :: cin >> n;
}