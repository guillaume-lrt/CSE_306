#include "sphere.hpp"

Intersection Sphere::intersect(const Ray& r){
    // return the intersection point between the sphere and the ray
    double t = 0;
    bool is_inter = true;
    Vector u = r.direction;
    Vector O = r.origin;
    Vector C = this->center;
    double R = this->radius;
    Vector OC = C-O;
    auto t_temp = dot(u, OC);
    Vector projection = t_temp*u - OC;         // projection center of the sphere onto the ray
    if (norm(projection) > R){                  // ray can't intercept the sphere
        return Intersection();
    }
    auto delta = pow(t_temp, 2) - ((pow(norm(OC), 2)) - pow(R, 2));
    if (delta < 0){         // if not solutions
        is_inter = false;
        return Intersection();
    }
    else{
        auto t1 = t_temp - sqrt(delta);
        auto t2 = t_temp + sqrt(delta);
        if (t2 < 0){
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
        return Intersection(is_inter, vec, normal, distance,index);
    }
}