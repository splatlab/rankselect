CC = gcc
CPP = gcc -E
CXX = g++ -std=c++11
CXXCPP = g++ -E

# Flags passed to the C++ compiler.
#CXXFLAGS += -g -Wall -Wextra -I. -Iinclude/ -Iinclude/sdsl 
CXXFLAGS += -O9 -mpopcnt -I. -Iinclude/ -Iinclude/sdsl 

%.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

all: bitmap_bench

clean:
	rm -f bitmap_bench bitmap_bench_sdsl *.o

bitmap_bench: bitmap.o bitmap_bench.o
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) $^ -o $@

bitmap_bench_sdsl: bitmap.o ram_fs.o bits.o memory_management.o util.o bitmap_bench_sdsl.o
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) $^ -o $@
