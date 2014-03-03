COMPILE=clang++

SOURCE_DIR=src
COMMON=${SOURCE_DIR}/rank-tree.cpp
COMMON+=${SOURCE_DIR}/stack-simulator.cpp

HEADERS=${SOURCE_DIR}/rank-tree.hpp
HEADERS+=${SOURCE_DIR}/stack-simulator.hpp

OPTIONS=-O3


stack-distance: ${COMMON} ${HEADERS} ${SOURCE_DIR}/main.cpp
	${COMPILE} ${COMMON} ${OPTIONS} \
	 ${SOURCE_DIR}/main.cpp \
	 -I${SOURCE_DIR} --std=c++11 -o stack-distance


tests: testdir/stack-simulator-test testdir/rank-tree-test
	./testdir/rank-tree-test
	./testdir/stack-simulator-test


testdir/stack-simulator-test: testdir ${COMMON} ${HEADERS} ${SOURCE_DIR}/stack-simulator-test.cpp
	${COMPILE} ${COMMON} ${OPTIONS} \
	 ${SOURCE_DIR}/stack-simulator-test.cpp \
	 -I${SOURCE_DIR} --std=c++11 -o testdir/stack-simulator-test

testdir/rank-tree-test: testdir ${COMMON} ${HEADERS} src/rank-tree-test.cpp
	${COMPILE} ${COMMON} ${OPTIONS} \
	 ${SOURCE_DIR}/rank-tree-test.cpp \
	 -I${SOURCE_DIR} --std=c++11 -o testdir/rank-tree-test

testdir:
	mkdir -p testdir

clean-all:
	rm -rf testdir
	rm stack-distance