#include <iostream>
#include "vector.hpp"
#include "sphere.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "camera.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "vector"

int main()
{
    Sphere s = Sphere(Vector(0,0,0), 10);
    Sphere s_green = Sphere(Vector(0, 0, -1000), 940);
    Sphere s_blue = Sphere(Vector(0, -1000, 0), 990);
    Sphere s_magenta = Sphere(Vector(0, 0, 1000), 940);
    Sphere s_red = Sphere(Vector(0, 1000, 0), 940);
    std::vector<Sphere> scene = {s_green, s_blue, s_magenta, s_red};
    Vector Q = Vector(0,0,55);          // center of camera
    double alpha = 60;                  // field of view
    int W = 120*4;
    int H = 120*4;
    int channels_num = 3;
    Camera cam = Camera(Q,alpha,W,H);
    // Ray r = Ray(Q,Vector(0,0,-1));
    // auto inter = s.intersection(r);

    unsigned char data[W * H * channels_num];

    int index = 0;
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            int color = 1;
            auto direction = normalization(cam.pixel(j,H-i-1));
            Ray r = Ray(Q,direction);
            // auto inter = s.in;
            auto is_inter = s.intersection(r);
            if (is_inter[0] == inf) color = 0;
            data[index++] = (unsigned char)(color * 255.0);
            data[index++] = (unsigned char)(color * 255.0);
            data[index++] = (unsigned char)(color * 255.0);
        }
    }

    stbi_write_jpg("image.jpg", W, H, channels_num, data, W * channels_num);
    std::cout << "return";
    return 0;
}
