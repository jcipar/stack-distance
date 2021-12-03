#include <cstdlib>
#include <iostream>
#include <sys/time.h>
#include <vector>

#include <rank-tree.hpp>
#include <unit.hpp>


double benchmark();
double do_benchmark(int64_t N);
void rankTest();

int main(void) {
	srand((int)time(NULL));

	RankTree tree;
	tree.check();
	test(tree.first() == nullptr);

	auto hello = tree.Insert("hello");
	tree.check();
	test(tree.first() == hello);
	test(tree.last() == hello);
	test(hello->Rank() == 0);

	auto world = tree.Insert("world");
	tree.check();
	test(tree.first() == world);
	test(tree.last() == hello);
	test(hello->Rank() == 1);
	test(world->Rank() == 0);

	auto foo = tree.Insert("foo");
	tree.check();
	test(tree.first() == foo);
	test(tree.last() == hello);
	test(hello->Rank() == 2);
	test(world->Rank() == 1);
	test(foo->Rank() == 0);

	tree.Remove(world);
	test(tree.computeSize() == 2);
	test(hello->Rank() == 1);
	test(foo->Rank() == 0);

	tree.Remove(hello);
	tree.check();
	test(tree.last() == foo);
	test(tree.first() == foo);

	tree.InsertNode(hello);
	tree.check();
	test(tree.first() == hello);
	test(tree.last() == foo);

	rankTest();

	cout << benchmark() << " operations per second\n";
	return 0;
}


void rankTest() {
	int N = 5;
	vector<RankTreeNode*> nodes;
	RankTree tree;
	for (int i=0; i<N; i++) {
		string s = to_string(i);
		nodes.push_back(tree.Insert(s));
	}
	tree.check();
	for (int i=0; i<N; i++) {
		test(nodes[i]->Rank() == (N-i - 1));
	}
}


double dtime() {
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (double)tv.tv_sec + (double)tv.tv_usec/1e6;
}


double benchmark() {
	int64_t N = 1000000;
	do {
		double tt = do_benchmark(N);
		if (tt >= 1.0) {
			return N / tt;
		}
		N *= 2;
	} while (N < 1e8);
	return -1.0;
}


double do_benchmark(int64_t N) {
	int64_t M = 1000000;
	RankTree tree;
	vector<RankTreeNode*> nodes;
	for (int i=0; i<M; i++) {
		string s = to_string(rand() % M);
		nodes.push_back(tree.Insert(s));
	}
	double tt = dtime();
	for (int i=0; i<N; i++) {
		int si = rand() % M;
		if (nodes[si] != nullptr) {
			tree.Remove(nodes[si]);
			delete nodes[si];
			nodes[si] = nullptr;
		} else {
			string s = to_string(si);
			nodes[si] = tree.Insert(s);
		}
	}
	tt = dtime() - tt;
	tree.check();
	return tt;
}

