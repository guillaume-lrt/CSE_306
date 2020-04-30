#pragma once

#include "vector.hpp"
#include "utils.hpp"
#include "Monte_carlo.hpp"

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
        Vector pixel(int x, int y,bool muller = false){
            double f = this->width / (2*tan(dtr*this->fov/2));
            if (muller){
                auto [x_shift,y_shift] = boxMuller(1);
                return Vector(this->center[0] + x + 0.5 - this->width / 2 + x_shift, 
                        this->center[1] + y + 0.5 - this->height / 2 + y_shift, 
                        this->center[2] - f);
            }
            return Vector(this->center[0] + x + 0.5 - this->width / 2,
                          this->center[1] + y + 0.5 - this->height / 2,
                          this->center[2] - f);
        }
};