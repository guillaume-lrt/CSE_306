#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <chrono>

#include "scene.hpp"
#include "scene.cpp"
// #include "simple_obj_file_reader.hpp"

using namespace std::chrono;


int main(int argc, char **argv){
    auto start = high_resolution_clock::now();

    // Sphere s_right_1 = Sphere(Vector(21, 0, 0), 9, Vector(1, 1, 1), "transparent",1.5,true);
    Geometry* s_left = new Sphere(Vector(-21,0,0), 10, Vector(1,1,1),"mirror");
    Geometry* s_middle = new Sphere(Vector(0, 0, 0), 10, Vector(1, 1, 1));
    Geometry* s_right_2 = new Sphere(Vector(21, 0, 0), 10, Vector(1, 1, 1), "transparent",1.5);
    Geometry* s_green = new Sphere(Vector(0, 0, -1000), 940, Vector(0, 1, 0));
    Geometry* s_blue = new Sphere(Vector(0, -1000, 0), 990, Vector(0, 0, 1));
    Geometry* s_magenta = new Sphere(Vector(0, 0, 1000), 940, Vector(1,0,1));
    Geometry* s_red = new Sphere(Vector(0, 1000, 0), 940,Vector(1, 0, 0));
    Geometry* s_cyan = new Sphere(Vector(-1000, 0, 0), 940, Vector(0, 1, 1));
    Geometry* s_yellow = new Sphere(Vector(1000, 0, 0), 940, Vector(1, 1, 0));

    // Geometry* cat = new TriangleMesh();
    // TriangleMesh* cat = new TriangleMesh("cat_model/cat.obj");
    // cat->readOBJ("cat_model/cat.obj");

    Vector Q = Vector(0,0,55);          // center of camera
    double alpha = 60;                  // field of view
    const int W = 520;
    const int H = 520;
    Camera cam = Camera(Q,alpha,W,H);
    // Light L = Light(Sphere(Vector(-10,20,40),2),pow(10,5));
    Light L = Light(Vector(-10,20,40),pow(10,5));
    int max_path_length = 5;
    int K = 36;

    Scene scene = Scene({s_middle, s_left, s_right_2, s_green, s_blue, s_magenta, s_red, s_cyan, s_yellow}, L);
    // Scene scene = Scene({(Geometry*)cat},L);

    unsigned char image[W * H * 3];

    // #pragma omp parallel for schedule(static,1)
    for (int i = 0; i < H; i++){
        // #pragma omp parallel for
        for (int j = 0; j < W; j++){
            Vector color = Vector(0,0,0);
            bool muller = false;         // if using muller box for antialiasing

            // std::vector<double> index = {1};
            // color = scene.getColor(r, max_path_length);
            // std::cout << *index.size() << std::endl;

            Vector ave_color = Vector(0,0,0);
            for (int u = 0; u < K; u++){
                auto direction = cam.pixel(j, H - i - 1, muller) - Q;
                Ray r = Ray(Q, direction);

                color = scene.getColor(r,max_path_length);
                // print(color);
                // std::cout << color[0] << ", " << color[1] << ", " << color[2] << std::endl;
                ave_color = ave_color + color;
            }
            color = ave_color/K;
            // print(color);

            double power = 1. / 2.2;
            image[(i * W + j) * 3 + 0] = std::min(255., std::max(0., pow(color[0], power) * 255));
            image[(i * W + j) * 3 + 1] = std::min(255., std::max(0., pow(color[1], power) * 255));
            image[(i * W + j) * 3 + 2] = std::min(255., std::max(0., pow(color[2], power) * 255));
            }
    }

    stbi_write_jpg("image.jpg", W, H, 3, image, 0);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    duration = duration / 1000000;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    return 0;
}
