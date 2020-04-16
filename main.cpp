#include <iostream>
#include "vector.hpp"
#include "sphere.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "light.hpp"

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
    int W = 520;
    int H = 520;
    int channels_num = 3;
    Camera cam = Camera(Q,alpha,W,H);
    Light L = Light(Vector(-10,20,40),pow(10,10));
    
    // print(normalization(Vector(-1,-2,-5)));

    unsigned char data[W * H * channels_num];

    int index = 0;
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            int color = 0;
            auto direction = cam.pixel(j,H-i-1)-Q;
            Ray r = Ray(Q,direction);
            auto inter = s.intersect(r);
            if (inter.is_intersection){ 
                color = 1;
                Vector N = inter.normal;
                Vector P = inter.position;
                Vector S = L.origin;
                double distance_light = norm(S - P);
                Vector omega = (S-P)/distance_light;
                Ray r_light = Ray(S,omega);
                double visibility = s.intersect(r_light).is_intersection ? 0 : 1;
            }
            data[index++] = (unsigned char)(color * 255.0);
            data[index++] = (unsigned char)(color * 255.0);
            data[index++] = (unsigned char)(color * 255.0);
        }
    }

    stbi_write_jpg("image.jpg", W, H, channels_num, data, W * channels_num);
    std::cout << "return";
    return 0;
}
