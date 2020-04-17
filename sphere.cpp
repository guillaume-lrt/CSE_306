#include "sphere.hpp"

Intersection Sphere::intersect(const Ray& r)
{
    // return the intersection point between the sphere and the ray
    double t = 0;
    bool is_inter = true;
    Vector u = r.direction;
    Vector O = r.origin;
    Vector C = this->center;
    double R = this->radius;
    auto delta = pow(dot(u, O - C), 2) - ((pow(norm(O - C), 2)) - pow(R, 2));
    // std::cout << "delta: " << delta << std::endl;
    if (delta < 0)
    { // if not solutions
        is_inter = false;
        return Intersection();
    }
    else
    {
        auto t_temp = dot(u, C - O);
        auto t1 = t_temp - sqrt(delta);
        auto t2 = t_temp + sqrt(delta);
        if (t2 < 0)
        {
            is_inter = false;
            return Intersection();
        }
        else if (t1 >= 0)
            t = t1;
        else
            t = t2;
        double distance = t;
        Vector vec = O + t * u;

        auto temp = vec - C;
        Vector normal = temp / norm(temp);
        return Intersection(is_inter, vec, normal, distance);
    }
}