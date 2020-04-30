#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <chrono>

#include "vector.hpp"
#include "sphere.hpp"
#include "sphere.cpp"
#include "ray.hpp"
#include "scene.hpp"
#include "scene.cpp"
#include "camera.hpp"
#include "light.hpp"
#include "Monte_carlo.hpp"

using namespace std::chrono;


int main(int argc, char **argv){
    auto start = high_resolution_clock::now();

    Sphere s_left = Sphere(Vector(-21,0,0), 10, Vector(1,1,1),"mirror");
    Sphere s_middle = Sphere(Vector(0, 0, 0), 10, Vector(1, 1, 1));
    // Sphere s_right_1 = Sphere(Vector(21, 0, 0), 9, Vector(1, 1, 1), "transparent",1.5,true);
    Sphere s_right_2 = Sphere(Vector(21, 0, 0), 10, Vector(1, 1, 1), "transparent",1.5);
    Sphere s_green = Sphere(Vector(0, 0, -1000), 940, Vector(0, 1, 0));
    Sphere s_blue = Sphere(Vector(0, -1000, 0), 990, Vector(0, 0, 1));
    Sphere s_magenta = Sphere(Vector(0, 0, 1000), 940, Vector(1,0,1));
    Sphere s_red = Sphere(Vector(0, 1000, 0), 940,Vector(1, 0, 0));
    Sphere s_cyan = Sphere(Vector(-1000, 0, 0), 940, Vector(0, 1, 1));
    Sphere s_yellow = Sphere(Vector(1000, 0, 0), 940, Vector(1, 1, 0));

    Vector Q = Vector(0,0,55);          // center of camera
    double alpha = 60;                  // field of view
    const int W = 520;
    const int H = 520;
    Camera cam = Camera(Q,alpha,W,H);
    Light L = Light(Vector(-10,20,40),pow(10,5));
    int max_path_length = 5;
    int K = 100;

    Scene scene = Scene({s_middle, s_left, s_right_2, s_green, s_blue, s_magenta, s_red, s_cyan, s_yellow}, L);

    unsigned char data[W * H * 3];

#pragma omp parallel for schedule(static,1)
    for (int i = 0; i < H; i++){
        // #pragma omp parallel for
        for (int j = 0; j < W; j++){
            Vector color = Vector(0,0,0);
            bool muller = false;         // if using muller box for antialiasing
            auto direction = cam.pixel(j,H-i-1,muller)-Q;
            Ray r = Ray(Q,direction);

            // std::vector<double> index = {1};
            // color = scene.getColor(r, max_path_length);
            // std::cout << *index.size() << std::endl;

            Vector ave_color = Vector(0,0,0);
            for (int i = 0; i < K; i++){
                color = scene.getColor(r,max_path_length);
                ave_color = ave_color + color;
            }
            color = ave_color/K;
            
            double power = 1. / 2.2;
            data[(i * W + j) * 3 + 0] = std::min(255., std::max(0., pow(color[0], power) * 255));
            data[(i * W + j) * 3 + 1] = std::min(255., std::max(0., pow(color[1], power) * 255));
            data[(i * W + j) * 3 + 2] = std::min(255., std::max(0., pow(color[2], power) * 255));
            }
    }

    stbi_write_jpg("image.jpg", W, H, 3, data, 0);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    duration = duration / 1000000;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    return 0;
}
