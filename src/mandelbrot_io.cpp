#include "../include/mandelbrot.hpp"
#include <fstream>

void save_ppm(const std::vector<int>& image, const std::string& filename) {
    std::ofstream out(filename);
    
    // PPM Header: P3 means ASCII color, then dimensions, then max color value
    out << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    for (int iter : image) {
        if (iter == MAX_ITER) {
            // Points inside the set are black
            out << "0 0 0 ";
        } else {
            // Simple architectural "Heat Map" coloring
            int r = (iter * 7) % 255;
            int g = (iter * 5) % 255;
            int b = (iter * 11) % 255;
            out << r << " " << g << " " << b << " ";
        }
    }
    out.close();
}