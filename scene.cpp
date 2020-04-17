#include "scene.hpp"
#include "sphere.hpp"
// #include "light.hpp"
#include "math.h"

Intersection Scene::intersection(const Ray& r){
    int n = this->spheres.size();
    double min_d = inf;
    Intersection res;
    for (int i = 0;i<n;i++){
        auto temp = this->spheres[i].intersect(r);
        if (temp.is_intersection){
            if(temp.distance < min_d){
                min_d = temp.distance;
                res = temp;
                std::cout << i << std::endl;
            }
        }
    }
    // std::cout << res.index << std::endl;
    return res;
}

Vector Scene::intensity_reflexion(Light lght,const Ray& r){
    double I = lght.intensity;
    Vector S = lght.origin;
    Intersection inter = intersection(r);
    Vector N = inter.normal;
    Vector P = inter.position + N*0.01;
    Sphere s = spheres[inter.index];
    Vector albedo = s.albedo;
    double d = norm(S-P);
    Vector omega = (S - P) / d;
    Ray r_light = Ray(S, omega);
    int visibility = s.intersect(r_light).distance > d ? 1 : 0;

    auto temp = I / (4 * PI * pow(d, 2));
    auto temp1 = albedo / PI;
    auto temp2 = visibility;
    auto temp3 =  dot(N, omega);
    // std::cout << "\ntest\n" << temp << std::endl;
    // print(s.center);
    // std::cout << temp2 << std::endl << temp3 << std::endl;
    Vector L = I / (4 * PI * pow(d, 2)) * albedo / PI * visibility * std::max(dot(N, omega),0.);
    // print(L);
    return L;
}