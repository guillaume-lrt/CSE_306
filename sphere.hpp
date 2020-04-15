#pragma once
#include "vector.hpp"
#include "ray.hpp"
#include <limits>

double inf = std::numeric_limits<double>::infinity();

class Sphere {
    public:
        Vector center;
        double radius;
        Vector albedo;
        explicit Sphere(Vector C, double R){
            center = C;
            radius = R;
        }

        // class intersection {
        // public:
        //     bool is_inter;
        //     Vector vec;
        //     double distance;
        //     Vector normal;

        //     intersection(Ray r)
        //     {
        //         // return the intersection point between the sphere and the ray
        //         double t = 0;
        //         auto u = r.direction;
        //         auto O = r.origin;
        //         auto C = Sphere::center;
        //         auto R = Sphere::radius;
        //         auto delta = pow(dot(u, O - C), 2) - ((pow(norm(O - C), 2)) - pow(R, 2));
        //         // std::cout << "delta: " << delta << std::endl;
        //         if (delta < 0)
        //         { // if not solutions
        //             is_inter = false;
        //         }
        //         else
        //         {
        //             auto t1 = dot(u, C - O) - sqrt(delta);
        //             auto t2 = dot(u, C - O) + sqrt(delta);
        //             if (t2 < 0)
        //             {
        //                 is_inter = false;
        //             }
        //             else if (t1 >= 0)
        //                 t = t1;
        //             else
        //                 t = t2;
        //             is_inter = true;
        //             distance = t;
        //             vec = O + t * u;

        //             auto temp = vec - C;
        //             normal = temp / norm(temp);
        //         }
        //     }
        // };

        Vector intersection(Ray r){
            // return the intersection point between the sphere and the ray
            double t = 0;
            auto u = r.direction;
            auto O = r.origin;
            auto C = this->center;
            auto R = this->radius;
            auto delta = pow(dot(u, O - C), 2) - ((pow(norm(O - C), 2)) - pow(R, 2));
            // std::cout << "delta: " << delta << std::endl;
            if (delta < 0)
            { // if not solutions
                return Vector(inf, 0, 0);
            }
            else
            {
                auto t1 = dot(u, C - O) - sqrt(delta);
                auto t2 = dot(u, C - O) + sqrt(delta);
                if (t2 < 0)
                {
                    return Vector(inf, 0, 0);
                }
                else if (t1 >= 0)
                    t = t1;
                else
                    t = t2;
                auto res = O + t * u;
                // std::cout << "inter" << std::endl;
                return res;
            }
        }
        Vector normal(Vector P){
            // P := intersection between Ray r and sphere
            auto temp = P - this->center;
            return temp / norm(temp);
        }
};