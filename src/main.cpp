#include <iostream>
using namespace std;

#include <stack-simulator.hpp>

// Quick-and-dirty command line parsing
bool hasFlag(string flag, int argc, char** argv);

int main(int argc, char** argv) {
	StackSimulator simulator;
	bool weights = hasFlag("--weight", argc, argv);
	while (not cin.eof()) {
		string s;
		uint64_t weight = 1;
		cin >> s;
		if (weights) {
			cin >> weight;
		}
		cout << simulator.Reference(s, weight) << endl;
	}
	return 0;
}


bool hasFlag(string flag, int argc, char** argv) {
	for (int i=1; i < argc; i++) {
		if (flag == argv[i]) {
			return true;
		}
	}
	return false;
}
