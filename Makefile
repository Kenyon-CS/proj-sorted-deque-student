# Simple C++17 build/test/submit
CXX := c++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra -Iinclude

BIN := bin/tests
SRC := src/SortedDeque.cpp tests/test_sorted_deque.cpp

.PHONY: all build test clean submit

all: build

build: $(BIN)

$(BIN): $(SRC) include/SortedDeque.h
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRC)

test: build
	@$(BIN)

submit: test
	@./submit.sh

clean:
	rm -f $(BIN)
