CC = gcc
CPP = gcc -E
CXX = g++ -std=c++11
CXXCPP = g++ -E

# Flags passed to the C++ compiler.
#CXXFLAGS += -g -mpopcnt -msse4 -Wall -Wextra -I. -Iinclude/ -Iinclude/sdsl 
CXXFLAGS += -O9 -mpopcnt -msse4 -I. -Iinclude/ -Iinclude/sdsl 

%.o: %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

all: bitmap_bench

clean:
	rm -f bitmap_bench bitmap_bench_sdsl bitmap_bench_stats bitmap_bench_word bitmap_bench_stats_dep *.o core

bitmap_bench: bitmap.o bitmap_bench.o
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) $^ -o $@

bitmap_bench_stats: bitmap.o bitmap_bench_stats.o
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) $^ -o $@

bitmap_bench_stats_dep: bitmap.o bitmap_bench_stats_dep.o
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) $^ -o $@

bitmap_bench_word: bitmap.o bitmap_bench_word.o
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) $^ -o $@

bitmap_bench_sdsl: bitmap.o ram_fs.o bits.o memory_management.o util.o bitmap_bench_sdsl.o
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) $^ -o $@
