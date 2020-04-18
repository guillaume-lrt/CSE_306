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
    if (ray_depth < 0) {
        return Vector(0.,0.,0.);
        }
    Intersection inter = this->intersection(r);
    Vector N = inter.normal;
    Vector P = inter.position + N*0.01;
    // if (P[0] == inter.position[0]) std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    Sphere s = spheres[inter.index];
    Vector dir = r.direction;

    if (inter.is_intersection){
        if (s.mirror){
            Ray reflect = Ray(P,dir - 2*dot(dir,N)*N);
            return intensity_reflexion(reflect,ray_depth-1);
        }
        else if (s.transparent){
            double n1 = 1;
            double n2 = s.refract_index;
            if (dot(dir, N) > 0)
            { // ray inside the sphere
                // P = P + 2 * 0.001 * N;
                // N = Vector(0., 0., 0.) - N;
                // n1 = 1.5;
                // n2 = 1;
                // std::cout << std::endl;
                N = -N;
                P = inter.position + N*0.01;
                n1 = s.refract_index;
                n2 = 1;
            }
            Vector r_tan = n1/n2 * (dir - dot(dir,N)*N);
            Vector r_nor = -N*sqrt(1-pow(n1/n2,2)*(1-pow(dot(dir,N),2)));

            Ray refract = Ray(P-N*0.02,r_tan + r_nor);
            // print(refract.origin);
            // std::cout << inter.index << std::endl;
            return intensity_reflexion(refract,ray_depth-1);
        }
        else{
            double I = light.intensity;
            Vector S = light.origin;
            
            Vector albedo = s.albedo;
            double d = norm(S - P);
            Vector omega = (S - P) / d;
            Ray r_light = Ray(S, -omega);
            int visibility = this->intersection(r_light).distance > d ? 1 : 0;

            return I / (4 * PI * pow(d, 2)) * albedo / PI * visibility * std::max(dot(N, omega), 0.);
        }
    }
    // std::cout << " " << std::endl;
    // print(r.origin);
    // print(r.direction);
    return Vector(0.,0.,0.);
}