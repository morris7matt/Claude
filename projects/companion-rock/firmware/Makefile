# Host-side tests for the behavior logic. No hardware or Arduino needed.
#   make test
CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Werror -O1 -Ilib/behavior

.PHONY: test clean
test: build/test_behavior
	./build/test_behavior

build/test_behavior: test/test_behavior.cpp lib/behavior/behavior.cpp lib/behavior/behavior.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) test/test_behavior.cpp lib/behavior/behavior.cpp -o $@

clean:
	rm -rf build
