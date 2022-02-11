#include <iostream>
#include <test.h>
#include <fstream>

int main() {
    std::ofstream renderImage("render.ppm");

    int nx = 200;
    int ny = 100;

    renderImage << "P3\n" << nx << " " << ny << "\n255\n";

    for (int j  = ny-1; j>=0; j--){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i <nx; i++) {
            float r = float(i) / float(nx);
            float g = float(j) / float (ny);
            float b = 0.2f;

            int ir = int(255.99*r);
            int ig = int(255.99*g);
            int ib = int(255.99*b);
            renderImage << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nDone.\n";
    renderImage.close();
    system("pause");
    return 0;
}