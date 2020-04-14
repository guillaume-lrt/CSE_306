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

void save_image(const int w, const int h, int channels_num){
    unsigned char data[w * h * channels_num];

    int index = 0;
    for (int j = h - 1; j >= 0; --j)
    {
        for (int i = 0; i < w; ++i)
        {
            data[index++] = (unsigned char)(255.0 * i / w);
            data[index++] = (unsigned char)(255.0 * j / h);
            data[index++] = (unsigned char)(255.0 * 0.2);
        }
    }

    stbi_write_jpg("jpg_test_.jpg", w, h, channels_num, data, w * channels_num);
}

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
    int W = 1080;
    int H = 720;
    int channels_num = 3;
    Camera cam = Camera(Q,alpha,W,H);
    Ray r = Ray(Q,Vector(0,0,-1));
    auto inter = s.intersection(r);
    // save_image(128, 128, 3);
    

    return 0;
}
