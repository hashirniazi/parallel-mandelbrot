#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <vector>
#include <string>

// Architecture Parameters
const int WIDTH = 1600;
const int HEIGHT = 1200;
const int MAX_ITER = 1000;

// Function declarations
void compute_mandelbrot_parallel(std::vector<int>& image);
void compute_mandelbrot_serial(std::vector<int>& image);
void save_ppm(const std::vector<int>& image, const std::string& filename);

#endif