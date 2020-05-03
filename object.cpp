#include "object.hpp"

Intersection Sphere::intersect(const Ray& r){
    // return the intersection point between the sphere and the ray
    double distance = 0;
    bool is_inter = true;
    Vector u = r.direction;
    Vector O = r.origin;
    Vector C = this->center;
    double R = this->radius;
    Vector OC = C-O;
    auto t_temp = dot(u, OC);
    // Vector projection = t_temp*u - OC;         // projection center of the sphere onto the ray
    // if (norm(projection) > abs(R)){                  // ray can't intercept the sphere
    //     return Intersection();
    // }
    auto delta = pow(t_temp, 2) - (norm_square(OC) - pow(R, 2));
    if (delta < 0){         // if not solutions
        is_inter = false;
        return Intersection();
    }
    else{
        delta = sqrt(delta);
        auto t1 = t_temp - delta;
        auto t2 = t_temp + delta;
        if (t2 < 0){
            is_inter = false;
            return Intersection();
        }
        else if (t1 >= 0)
            distance = t1;
        else
            distance = t2;
        // double distance = t;
        Vector position = O + distance * u;

        // auto temp = ;
        Vector normal = position - C / norm(position - C);
        return Intersection(is_inter, position, normal, distance,index);
    }
}

Intersection TriangleMesh::intersect(const Ray &r){
    Vector O = r.origin;
    Vector u = r.direction;
    double min_d = inf;
    Intersection res;
    for (auto &triangle : this->indices){
        Vector A = vertices[triangle.vtxi];
        Vector B = vertices[triangle.vtxj];
        Vector C = vertices[triangle.vtxk];
        Vector e1 = B - A;
        Vector e2 = C - A;
        Vector N = cross(e1,e2);
        double uN = dot(u,N);
        if (uN != 0){
            Vector AOu = cross(A-O,u);
            double beta = dot(e2,AOu)/uN;
            double gamma = - dot(e1,AOu)/uN;
            double alpha = 1 - beta- gamma;
            double distance = dot(A-O,N)/uN;        // <=> t
            Vector position = O + distance*u;
            if (abs(alpha) <= 1 && abs(beta) <= 1 && abs(gamma) <= 1 && distance < min_d){
                min_d = distance;
                res = Intersection(true, position, normalization(N), distance, index);
                // return Intersection(true,position,N,distance,index);
            }
        }
    }
    return res;
}