#include "scene.hpp"
#include "sphere.hpp"
// #include "light.hpp"
#include "math.h"

Intersection Scene::intersection(const Ray& r){
    int n = spheres.size();
    double min_d = inf;
    Intersection res;
    for (int i = 0;i<n;i++){
        auto temp = spheres[i].intersect(r);
        if (temp.is_intersection){
            if(temp.distance < min_d){
                min_d = temp.distance;
                res = temp;
            }
        }
    }
    return res;
}

Vector Scene::intensity_reflexion(const Ray& r, int ray_depth){
    if (ray_depth < 0) return Vector(0.,0.,0.);
    Intersection inter = intersection(r);
    Vector N = inter.normal;
    Vector P = inter.position + N*0.001;

    if (inter.is_intersection){
        if (spheres[inter.index].mirror){
            Ray reflect = Ray(P,r.direction - 2*dot(r.direction,N)*N);
            return intensity_reflexion(reflect,ray_depth-1);
        }
        else{
            double I = light.intensity;
            Vector S = light.origin;
            
            Sphere s = spheres[inter.index];
            Vector albedo = s.albedo;
            double d = norm(S - P);
            Vector omega = (S - P) / d;
            Ray r_light = Ray(S, -omega);
            int visibility = this->intersection(r_light).distance > d ? 1 : 0;

            return I / (4 * PI * pow(d, 2)) * albedo / PI * visibility * std::max(dot(N, omega), 0.);
        }
    }
    return Vector(0.,0.,0.);
}