#pragma once
#include "scene.hpp"

#include "math.h"
#include <omp.h>

#include <random>

#include "sphere.hpp"
// #include "light.hpp"


static std::default_random_engine engine_scene[8];
// static std::default_random_engine engine_scene(10);
static std::uniform_real_distribution<double> uniform_scene(0, 1);

void Scene::add_sphere(Sphere S){
    this->spheres.push_back(S);
    S.index = this->spheres.size();
}

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

Vector Scene::random_cos(const Vector &N){
    double r1 = uniform_scene(engine_scene[omp_get_thread_num()]);
    double r2 = uniform_scene(engine_scene[omp_get_thread_num()]);
    
    double x = cos(2*PI*r1)*sqrt(1-r2);
    double y = sin(2*PI*r1)*sqrt(1-r2);
    double z = sqrt(r2);
    double p = z/PI;
    int smallest_index = 0;
    double mini = abs(N[0]);
    for (int i = 1; i < 3; i++){
        if (abs(N[i]) < mini){
            mini = abs(N[i]);
            smallest_index = i;
        }
    }
    Vector T1 = Vector(N[0],N[1],N[2]);
    T1[smallest_index] = 0;
    T1[(smallest_index + 1) % 3] *= -1;
    std::swap(T1[(smallest_index + 1) % 3], T1[(smallest_index + 2) % 3]);
    T1 = normalization(T1);
    Vector T2 = cross(N,T1);
    Vector V = x*T1+y*T2+z*N;
    return V;
}

Vector Scene::getColor(const Ray& r, int ray_depth){ //,std::vector<double> index){
    if (ray_depth < 0) {        // terminate the recursion
        return Vector(0.,0.,0.);
        }
    Intersection inter = this->intersection(r);
    Vector N = inter.normal;
    Vector P = inter.position + N*0.01;
    Sphere s = this->spheres[inter.index];
    Vector dir = r.direction;

    if (inter.is_intersection){
        if(s.light){
            return Vector(1,1,1) * this->light.intensity / (4*PI*PI*pow(s.radius,2));
        }
        if (s.mirror){        
            Ray reflect = Ray(P,dir - 2*dot(dir,N)*N);
            return getColor(reflect,ray_depth-1);
        }
        else if (s.transparent)
        {
            double n1;
            double n2;

            // if (s.hollow)
            // {
            //     N = -N;
            //     // P = inter.position + N * 0.01;
            //     n1 = 1.5;
            //     n2 = 1.5;
            // }

            if (dot(dir, N) > 0)
            {
                N = -N;
                P = inter.position + N * 0.01;
                
                n1 = s.refract_index; //index[0];
                n2 = 1; //index[1];
                
                // index.erase(index.begin()); // leaving the sphere => remove its refraction index from index
                // if (n1 != n2) std::swap(n1,n2);
            }
            else
            {
                // index.insert(index.begin(), s.refract_index); // enter the sphere => add its refraction index in the first position
                n1 = 1; //index[1];
                n2 = s.refract_index; //index[0];
            }

            auto n1n2 = n1 / n2;
            auto dot_dir = dot(dir, N);
            double radicand = 1 - pow(n1n2, 2) * (1 - pow(dot_dir, 2));

            auto k0 = pow(n1 - n2, 2) / pow(n1 + n2, 2);
            auto R = k0 + (1 - k0) * pow(1 - abs(dot(N,dir)), 5);
        
            auto T = 1 - R;
            // auto u = uniform_scene(engine_scene);
            auto u = uniform_scene(engine_scene[omp_get_thread_num()]);


            if (u>R && radicand >= 0){ // refract ray
                Vector r_tan = n1n2 * (dir - dot_dir * N);
                Vector r_nor = -N * sqrt(radicand);

                Ray refract = Ray(P - N * 0.02, r_tan + r_nor);

                return getColor(refract, ray_depth - 1);
            }
            Ray reflect = Ray(P, dir - 2 * dot_dir * N);
            return getColor(reflect, ray_depth - 1);
        }
        else{       // diffuse
            double I = this->light.intensity;
            Vector C = this->light.origin;
            double R = this->light.radius;
            double d = norm(C - P);     // distance between light center and intersection point

            Vector D = (P-C)/d;
            Vector V = random_cos(D);
            Vector xprime = R*V + C;            // point on the light sphere

            Vector Nprime = (xprime-C)/(R); //norm(xprime-C));
            Vector omega_i = (xprime-P)/(norm(xprime-P));
            
            Vector albedo = s.albedo;
        
            // Vector omega = (C - P) / d;
            // Ray r_light = Ray(C, -omega);
            Ray r_light = Ray(P,omega_i);
            // int visibility = this->intersection(r_light).distance > d ? 1 : 0;
            int visibility = this->intersection(r_light).distance > (d-R) ? 1 : 0;
            // int visibility = this->spheres[this->intersection(r_light).index].light ? 1 : 0;

            double pdf = dot(Nprime, D)/(PI*pow(R,2));

            // Vector Lo = I / (4 * PI * pow(d, 2)) * albedo / PI * visibility * std::max(dot(N, omega), 0.);
            Vector Lo = I / (4 * pow(PI,2) * pow(R, 2)) * albedo / PI * visibility * std::max(dot(N, omega_i), 0.) * std::max(dot(Nprime,-omega_i),0.)/(norm_square(xprime-P)*pdf);
            Ray random_ray = Ray(P,random_cos(N));
            Lo += albedo * getColor(random_ray,ray_depth-1);
            return Lo;
        }
    }
    return Vector(0.,0.,0.);
}