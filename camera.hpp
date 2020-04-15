#pragma once
#include "vector.hpp"

#define PI 3.14159265
#define rtd double(180 / PI) // radiantodegree: radian * rtd = degree
#define dtr double(PI / 180)

class Camera{
    public:
        Vector center;
        double fov;         // field of view
        double f;           // distance screen and center
        double width;
        double height;
        explicit Camera(Vector Q, double alpha, int W, int H)
        {
            center = Q;
            fov = alpha;
            width = W;
            height = H;
        }
        Vector pixel(int x, int y){
            double f = abs(this->width / (2*tan(this->fov/2)));
            // std::cout << f << std::endl;
            return Vector(this->center[0] + x + 0.5 - this->width / 2, 
                        this->center[1] + y + 0.5 - this->height / 2, 
                        this->center[2] - f);
        }
};