
#include <iostream>
using namespace std;

#include <stack-simulator.hpp>


int main(int argc, char** argv) {
    StackSimulator simulator;
    while (not cin.eof()) {
		string s;
		cin >> s;
		cout << simulator.Reference(s) << endl;
    }
    return 0;
}
