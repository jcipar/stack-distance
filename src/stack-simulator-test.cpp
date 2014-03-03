
#define __STDC_LIMIT_MACROS
#include <cstdint>
using namespace std;

#include <stack-simulator.hpp>
#include <unit.hpp>


int main(void) {
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

    return 0;
}
