CXX := g++
CXXFLAGS := -g -std=c++11

./build/lexer.hpp: ./build/lang.l
	flex --header-file=./build/lexer.hpp -o ./build/lexer.cpp ./build/lang.l

./build/lexer.cpp: ./build/lang.l
	flex -o ./build/lexer.cpp ./build/lang.l

./build/parser.cpp: ./build/lang.y
	bison -o ./build/parser.cpp --defines=./build/parser.hpp ./build/lang.y

./build/parser.hpp: ./build/lang.y
	bison -o ./build/parser.cpp -d -v ./build/lang.y

./build/lang.o: ./build/lang.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/lib.o: ./build/lib.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/lexer.o: ./build/lexer.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/parser.o: ./build/parser.cpp ./build/lexer.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/front.o: ./build/front.cpp ./build/lexer.hpp ./build/parser.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./bin/front: ./build/front.o ./build/parser.o ./build/lexer.o ./build/lib.o ./build/lang.o
	$(CXX) $(CXXFLAGS) $^ -o $@

./build/value.o: ./build/value.cpp ./build/value.hpp ./build/basictype.hpp ./build/lang.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/basictype.o: ./build/basictype.cpp ./build/basictype.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/symboltable.o: ./build/symboltable.cpp ./build/value.hpp ./build/basictype.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/interpreter.o: ./build/interpreter.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/main.o: ./build/main.cpp ./build/lexer.hpp ./build/parser.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./bin/main: ./build/main.o ./build/lib.o ./build/lexer.o ./build/parser.o ./build/lang.o ./build/interpreter.o ./build/value.o ./build/basictype.o ./build/symboltable.o
	$(CXX) $(CXXFLAGS) $^ -o $@

all: ./bin/main

test: ./bin/front

clean:
	rm -f ./build/lexer.* ./build/parser.* ./build/*.o

%.c: %.y

%.c: %.l

.DEFAULT_GOAL := all

