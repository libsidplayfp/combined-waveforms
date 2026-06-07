.PHONY: all clean

# Uncomment to enable parallel processing
FLAG_OPENMP = -fopenmp

CXXFLAGS = -march=native -O3

all: clean combined

clean:
	$(RM) combined

%: %.cpp
	$(CXX) $(CXXFLAGS) $(FLAG_OPENMP) -std=c++11 $< -o $@
