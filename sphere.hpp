#pragma once
#include "vector.hpp"
#include "ray.hpp"
#include <limits>

double inf = std::numeric_limits<double>::infinity();


class Intersection {
    public:
        bool is_intersection;
        Vector position;
        Vector normal;
        double distance;
        Intersection(bool b, Vector P,Vector N, double d){
            is_intersection = b;
            position = P;
            normal = N;
            distance = d;
        }
};

class Object{
    public:
        virtual Intersection intersect(const Ray r) = 0;
};

class Sphere : public Object {
    public:
        Vector center;
        double radius;
        Vector albedo;
        explicit Sphere(Vector C, double R,Vector albedo=Vector(0,0,0)){
            center = C;
            radius = R;
        }

        Intersection intersect (const Ray r) {
            // return the intersection point between the sphere and the ray
            double t = 0;
            bool is_inter = true;
            auto u = r.direction;
            auto O = r.origin;
            auto C = this->center;
            auto R = this->radius;
            auto delta = pow(dot(u, O - C), 2) - ((pow(norm(O - C), 2)) - pow(R, 2));
            // std::cout << "delta: " << delta << std::endl;
            if (delta < 0) { // if not solutions
                is_inter = false;
                return Intersection(is_inter, Vector(0,0,0), Vector(0,0,0), inf);
            }
            else {
                auto t_temp = dot(u, C - O);
                auto t1 = t_temp -sqrt(delta);
                auto t2 = t_temp + sqrt(delta);
                if (t2 < 0)  {
                    is_inter = false;
                    return Intersection(is_inter, Vector(0, 0, 0), Vector(0, 0, 0), inf);
                }
                else if (t1 >= 0)
                    t = t1;
                else
                    t = t2;
                double distance = t;
                Vector vec = O + t * u;

                auto temp = vec - C;
                Vector normal = temp / norm(temp);
                return Intersection(is_inter,vec,normal,distance);
            }
        }
};