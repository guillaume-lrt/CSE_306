#include "scene.hpp"
#include "sphere.hpp"
// #include "light.hpp"
#include "math.h"
#include <random>

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

Vector Scene::getColor(const Ray& r, int ray_depth, std::vector<double> index){
    if (ray_depth < 0) {
        return Vector(0.,0.,0.);
        }
    Intersection inter = this->intersection(r);
    Vector N = inter.normal;
    Vector P = inter.position + N*0.01;
    Sphere s = spheres[inter.index];
    Vector dir = r.direction;

    if (inter.is_intersection){
        if (s.mirror){
            Ray reflect = Ray(P,dir - 2*dot(dir,N)*N);
            return getColor(reflect,ray_depth-1);
        }
        else if (s.transparent){
            double n1;
            double n2;
            if (dot(dir, N) > 0){
                N = -N;
                P = inter.position + N*0.01;
                n1 = index[0];
                n2 = index[1];
                index.erase(index.begin());             // leaving the sphere => remove its refraction index from index
                // if (n1 != n2) std::swap(n1,n2);
            }
            else{
                index.insert(index.begin(), s.refract_index);       // enter the sphere => add its refraction index in the first position
                n1 = index[1];
                n2 = index[0];
            }

            // std::cout << n1 << ", " << n2 << std::endl;
            // if (n2 < 0){
            //     n2 = -n2;
            //     N = -N;
            // }

            // if (n1 < 0)
            // {
            //     n1 = -n1;
            //     N = -N;
            // }

            // if (inter.index == 2){
            //     std::cout << spheres[2].radius << std::endl;
            // }

            auto temp0 = n1 / n2;
            auto temp = dot(dir, N);

            auto k0 = pow(n1-n2,2)/pow(n1+n2,2);
            auto R = k0 + (1-k0)*pow(1-abs(temp),5);
            auto T= 1-R;
            auto u = ((double) rand() / (RAND_MAX));

            if (10 > R){                 // return refract ray                
                Vector r_tan = temp0 * (dir - temp * N);
                Vector r_nor = -N * sqrt(1 - pow(temp0, 2) * (1 - pow(temp, 2)));

                Ray refract = Ray(P - N * 0.02, r_tan + r_nor);

                return getColor(refract, ray_depth - 1, index);
            }
            Ray reflect = Ray(P, dir - 2 * temp * N);
            return getColor(reflect, ray_depth - 1);
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