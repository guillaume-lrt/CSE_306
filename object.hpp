#pragma once

#include "vector.hpp"
#include "Monte_carlo.hpp"

class Camera{
    public:
        Vector center;
        double fov; // field of view
        double f;   // distance screen and center
        double width;
        double height;
        explicit Camera(Vector Q, double alpha, int W, int H)
        {
            center = Q;
            fov = alpha;
            width = W;
            height = H;
        }
        Vector pixel(int x, int y, bool muller = false)
        {
            double f = this->width / (2 * tan(dtr * this->fov / 2));
            if (muller)
            {
                auto [x_shift, y_shift] = boxMuller(1);
                return Vector(this->center[0] + x + 0.5 - this->width / 2 + x_shift,
                            this->center[1] + y + 0.5 - this->height / 2 + y_shift,
                            this->center[2] - f);
            }
            return Vector(this->center[0] + x + 0.5 - this->width / 2,
                        this->center[1] + y + 0.5 - this->height / 2,
                        this->center[2] - f);
        }
};

class Light{
    public:
        Vector origin;
        double intensity;
        explicit Light(Vector C = Vector(0, 0, 0), double I = pow(10, 5))
        {
            origin = C;
            intensity = I;
        }
        // Sphere sphere;
        // double radius;
        // explicit Light(Sphere S = Sphere(), double I = pow(10,5)){ //Vector C, double I, double R){
        //     origin = S.center;
        //     intensity = I;
        //     radius = S.radius;
        //     sphere = S;
        //     sphere.light = true;
        // }
};

class Ray{
    public:
        Vector origin;
        Vector direction;
        explicit Ray(Vector O, Vector u)
        {
            origin = O;
            direction = normalization(u);
        }
};

class Intersection {
    public:
        bool is_intersection;
        Vector position;
        Vector normal;
        double distance;
        int index;
        bool hollow;
        Intersection(bool b = false, Vector P = Vector(0, 0, 0), Vector N = Vector(0, 0, 0), double d = inf, int i = 0){
            is_intersection = b;
            position = P;
            normal = N;
            distance = d;
            index = i;
            // hollow = h;
            // if (hollow) normal = -N;
        }
};

class Sphere {
    public:
        Vector center;
        double radius;
        Vector albedo;
        int index;
        bool mirror = false;
        bool transparent = false;
        // bool light = false;
        double refract_index;
        Intersection intersect(const Ray &r);
        bool hollow;

        explicit Sphere(Vector C = Vector(0,0,0), double R = 0,Vector A =Vector(0,0,0),std::string surface = "diffuse",double n = 0, bool h = false){
            center = C;
            radius = R;
            albedo = A;
            refract_index = n;
            if (surface == "mirror") mirror = true;
            if (surface == "transparent") transparent = true;
            hollow = h;
        }
};