
#define __STDC_LIMIT_MACROS
#include <cstdint>
using namespace std;

#include <stack-simulator.hpp>
#include <unit.hpp>

void randomizedTest();

int main(void) {
	srand(int(time(NULL)));
	StackSimulator simulator;

	test(simulator.Reference("hello") == INT64_MAX);
	simulator.check();

	test(simulator.Reference("hello") == 0);
	simulator.check();
	test(simulator.objectCount() == 1);

	test(simulator.Reference("world") == INT64_MAX);
	simulator.check();
	test(simulator.Reference("hello") == 1);
	simulator.check();
	test(simulator.objectCount() == 2);
	test(simulator.Reference("world") == 1);
	simulator.check();

	randomizedTest();
	return 0;
}

void randomizedTest() {
	int N = 100000;
	int M = 1000;
	StackSimulator simulator;
	for (int i = 1; i <= N; i++) {
		string s = to_string(rand() % M);
		simulator.Reference(s);
		if (N % 1000 == 0) {
			simulator.check();
		}
	}
	simulator.check();
}
