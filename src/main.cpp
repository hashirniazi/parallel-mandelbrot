#include "../include/mandelbrot.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <complex>
#include <omp.h>

// Logic for a single pixel to keep code clean
int get_iterations(double pr, double pi) {
    std::complex<double> c(pr, pi);
    std::complex<double> z(0, 0);
    int iter = 0;
    while (std::abs(z) < 2.0 && iter < MAX_ITER) {
        z = z * z + c;
        iter++;
    }
    return iter;
}

void compute_mandelbrot_parallel(std::vector<int>& image) {
    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double pr = 1.5 * (x - WIDTH / 2.0) / (0.5 * WIDTH);
            double pi = (y - HEIGHT / 2.0) / (0.5 * HEIGHT);
            image[y * WIDTH + x] = get_iterations(pr, pi);
        }
    }
}

void compute_mandelbrot_serial(std::vector<int>& image) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double pr = 1.5 * (x - WIDTH / 2.0) / (0.5 * WIDTH);
            double pi = (y - HEIGHT / 2.0) / (0.5 * HEIGHT);
            image[y * WIDTH + x] = get_iterations(pr, pi);
        }
    }
}

void compute_mandelbrot_simd(std::vector<int>& image) {
    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < HEIGHT; y++) {
        // Hint to the compiler to use vector registers (AVX/SSE)
        #pragma omp simd
        for (int x = 0; x < WIDTH; x++) {
            double pr = 1.5 * (x - WIDTH / 2.0) / (0.5 * WIDTH);
            double pi = (y - HEIGHT / 2.0) / (0.5 * HEIGHT);
            
            double zr = 0.0, zi = 0.0;
            int iter = 0;
            // Manual complex math is easier for the compiler to vectorize
            while (zr*zr + zi*zi < 4.0 && iter < MAX_ITER) {
                double next_zr = zr*zr - zi*zi + pr;
                double next_zi = 2.0 * zr * zi + pi;
                zr = next_zr;
                zi = next_zi;
                iter++;
            }
            image[y * WIDTH + x] = iter;
        }
    }
}

int main() {
    std::vector<int> image(WIDTH * HEIGHT);

    std::cout << "Starting benchmarks for " << WIDTH << "x" << HEIGHT << " image..." << std::endl;

    // --- Serial Execution ---
    auto start_s = std::chrono::high_resolution_clock::now();
    compute_mandelbrot_serial(image);
    auto end_s = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_s = end_s - start_s;
    std::cout << "Serial SISD Time:   " << diff_s.count() << " seconds" << std::endl;

    // --- MIMD Execution ---
    auto start_p = std::chrono::high_resolution_clock::now();
    compute_mandelbrot_parallel(image);
    auto end_p = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff_p = end_p - start_p;
    std::cout << "Parallel MIMD Time: " << diff_p.count() << " seconds (" << omp_get_max_threads() << " threads)" << std::endl;
    
    // --- SIMD Execution ---
    auto start_simd = std::chrono::high_resolution_clock::now();
    compute_mandelbrot_simd(image);
    auto end_simd = std::chrono::high_resolution_clock::now();
    
    // --- SIND Speedup Calculation ---
    std::chrono::duration<double> diff_simd = end_simd - start_simd;
    std::cout << "Parallel SIMD Time: " << diff_simd.count() << " seconds (" << omp_get_max_threads() << " threads)" << std::endl;
    
    // --- MIMD Speedup Calculation ---
    double speedup = diff_s.count() / diff_p.count();
    
    std::cout << "\nMIMD Speedup: " << speedup << "x" << std::endl;
    std::cout << "SIMD Speedup: " << diff_s.count() / diff_simd.count() << "x" << std::endl;

    save_ppm(image, "fractal.ppm");
    return 0;
}