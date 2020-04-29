#pragma once

#include <iostream>
#include <math.h>
#include <vector>

class Vector{
    private:
        double coords[3];

    public:
        explicit Vector(double x = 0., double y = 0., double z = 0.)
        {
            coords[0] = x;
            coords[1] = y;
            coords[2] = z;
            };
            Vector &operator+=(const Vector &b)
            {
                coords[0] += b[0];
                coords[1] += b[1];
                coords[2] += b[2];
                return *this;
            }
            Vector &operator*=(const double t)
            {
                coords[0] *= t;
                coords[1] *= t;
                coords[2] *= t;
                return *this;
            }
            const double &operator[](int i) const { return coords[i]; }
            double &operator[](int i) { return coords[i]; }
};

bool operator==(const Vector &a, const Vector &b){
    bool temp = ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2])) ? true : false;
    return temp;
}

Vector operator+(const Vector &a, const Vector &b){
    return Vector(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

Vector operator-(const Vector &a, const Vector &b)
{
    return Vector(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

Vector operator-(const Vector &b)
{
    return Vector(- b[0], - b[1], - b[2]);
}

double dot(const Vector &a, const Vector &b){
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double norm(const Vector &a){
    return sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
}

Vector cross(const Vector &a, const Vector &b){
    return Vector(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

Vector normalization(const Vector &a){
    double nrm = norm(a);
    return Vector(a[0] / nrm, a[1] / nrm, a[2] / nrm);
}

Vector operator*(const double t, const Vector &a){
    return Vector(t * a[0], t * a[1], t * a[2]);
}

Vector operator*(const Vector &a, const double t){
    return Vector(t * a[0], t * a[1], t * a[2]);
}

Vector operator*(const Vector &a, const Vector &b){
    return Vector(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

Vector operator/(const Vector &a, const double t){
    return Vector(a[0]/t, a[1]/t, a[2]/t);
}

Vector operator/(const double t, const Vector &a){
    return Vector(a[0] / t, a[1] / t, a[2] / t);
}

void print(Vector a){
    std::cout << "[" << a[0] << ", " << a[1] << ", " << a[2] << "]" << std::endl;
}

// Vector average(std::vector<Vector> colors){
//     size_t n = colors.size();
//     Vector res = Vector(0,0,0);
//     for(int i = 0; i < n; i++){
//         res = res + colors[i];
//     }
//     return res/n;
// }

// #endif