.PHONY: all clean

# Uncomment to enable parallel processing
FLAG_OPENMP = -fopenmp

CXXFLAGS = -march=native -O3

all: clean combined dump rms

clean:
	$(RM) combined dump rms

%: %.cpp
	$(CXX) $(CXXFLAGS) $(FLAG_OPENMP) -std=c++17 $< -o $@
