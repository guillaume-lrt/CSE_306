#include "scene.hpp"
#include "sphere.hpp"
// #include "light.hpp"
#include "math.h"
#include <random>
// #include <time.h>

// static std::default_random_engine engine[8];
static std::default_random_engine engine(10);
static std::uniform_real_distribution<double> uniform(0, 1);

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
    double r1 = uniform(engine);
    double r2 = uniform(engine);
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
    // print(N);
    T1[smallest_index] = 0;
    T1[(smallest_index + 1) % 3] *= -1;
    std::swap(T1[(smallest_index + 1) % 3], T1[(smallest_index + 2) % 3]);
    // std::cout << "[" << T1[0] << ", " << T1[1] << ", " << T1[2] << "]\n" << std::endl;
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
                
                n1 = 1.5; //index[0];
                n2 = 1; //index[1];
                
                // index.erase(index.begin()); // leaving the sphere => remove its refraction index from index
                // if (n1 != n2) std::swap(n1,n2);
            }
            else
            {
                // index.insert(index.begin(), s.refract_index); // enter the sphere => add its refraction index in the first position
                n1 = 1; //index[1];
                n2 = 1.5; //index[0];
            }

            auto n1n2 = n1 / n2;
            auto dot_dir = dot(dir, N);
            double radicand = 1 - pow(n1n2, 2) * (1 - pow(dot_dir, 2));

            auto k0 = pow(n1 - n2, 2) / pow(n1 + n2, 2);
            auto R = k0 + (1 - k0) * pow(1 - abs(dot(N,dir)), 5);
        
            auto T = 1 - R;
            auto u = uniform(engine);
            // uniform(engine[int omp_get_thread_num(void)]);


            if (u>R && radicand >= 0){ // refract ray
                Vector r_tan = n1n2 * (dir - dot_dir * N);
                Vector r_nor = -N * sqrt(radicand);

                Ray refract = Ray(P - N * 0.02, r_tan + r_nor);

                return getColor(refract, ray_depth - 1);
            }
            Ray reflect = Ray(P, dir - 2 * dot_dir * N);
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

            Vector Lo = I / (4 * PI * pow(d, 2)) * albedo / PI * visibility * std::max(dot(N, omega), 0.);
            Ray random_ray = Ray(P,random_cos(N));
            Lo += albedo * getColor(random_ray,ray_depth-1);
            return Lo;
        }
    }
    return Vector(0.,0.,0.);
}