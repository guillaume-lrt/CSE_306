#include <iostream>
#include "vector.hpp"
#include "sphere.hpp"
#include "sphere.cpp"
#include "ray.hpp"
#include "scene.hpp"
#include "scene.cpp"
#include "camera.hpp"
#include "light.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "vector"
#include "math.h"

int main()
{
    Sphere s = Sphere(Vector(0,0,0), 10, Vector(1,1,1));
    Sphere s_green = Sphere(Vector(0, 0, -1000), 940, Vector(0, 1, 0));
    Sphere s_blue = Sphere(Vector(0, -1000, 0), 990, Vector(0, 0, 1));
    Sphere s_magenta = Sphere(Vector(0, 0, 1000), 940, Vector(1,0,1));
    Sphere s_red = Sphere(Vector(0, 1000, 0), 940,Vector(1, 0, 0));
    Scene scene = Scene({s,s_green, s_blue, s_magenta, s_red});
    Vector Q = Vector(0,0,55);          // center of camera
    double alpha = 60;                  // field of view
    int W = 720;
    int H = 720;
    Camera cam = Camera(Q,alpha,W,H);
    Light L = Light(Vector(-10,20,40),pow(10,5));

    // print(s_magenta.albedo);
    
    unsigned char data[W * H * 3];

    int index = 0;
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            Vector color = Vector(0,0,0);
            auto direction = cam.pixel(j,H-i-1)-Q;
            Ray r = Ray(Q,direction);
            // auto inter = s.intersect(r);
            // if (inter.is_intersection){ 
            //     color = Vector(1,1,1);
            //     Vector N = inter.normal;
            //     Vector P = inter.position;
            //     Vector S = L.origin;
            //     double distance_light = norm(S - P);
            //     Vector omega = (S-P)/distance_light;
            //     Ray r_light = Ray(S,omega);
            //     double visibility = s.intersect(r_light).is_intersection ? 0 : 1;
            // }
            color = scene.intensity_reflexion(L,r);
            double power = 1. / 2.2;
            data[(i * W + j) * 3 + 0] = std::min(255., std::max(0., pow(color[0], power) * 255));
            data[(i * W + j) * 3 + 1] = std::min(255., std::max(0., pow(color[1], power) * 255));
            data[(i * W + j) * 3 + 2] = std::min(255., std::max(0., pow(color[2], power) * 255));
        }
    }

    stbi_write_jpg("image.jpg", W, H, 3, data, W * 3);
    std::cout << "return";
    return 0;
}
