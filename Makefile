CC = gcc
CPP = gcc -E
CXX = g++ -std=c++11
CXXCPP = g++ -E

# Flags passed to the C++ compiler.
#CXXFLAGS += -g -Wall -Wextra
CXXFLAGS += -O9 -mpopcnt -I../sdsl-lite/include/ -I../sdsl-lite/include/sdsl/

%.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

all: bitmap_bench

clean:
	rm -f bitmap_bench *.o

bitmap_bench: bitmap.o ram_fs.o bits.o memory_management.o util.o bitmap_bench.o
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) $^ -o $@
