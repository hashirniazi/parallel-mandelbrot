CXX = g++
CXXFLAGS = -O3 -fopenmp -march=native -ffast-math
SRC = ../src/main.cpp ../src/mandelbrot_io.cpp
TARGET = mandelbrot

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) output/*.ppm