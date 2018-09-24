CXX=/usr/local/opt/llvm/bin/clang++
CXXFLAGS=-Ofast -march=native -std=c++17 -fconstexpr-steps=1000000000

all: damen
	./damen

damen: damen.o
	$(CXX) $^ -o damen

clean:
	rm -f damen damen.o
