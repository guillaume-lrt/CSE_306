#pragma once
#include <iostream>
#include <random>
#include <tuple>

#include "utils.hpp"

static std::default_random_engine engine(10); // random seed = 10
static std::uniform_real_distribution<double> uniform(0, 1);

std::tuple<double, double> boxMuller(double stdev){
    double r1 = uniform(engine);
    double r2 = uniform(engine);
    double x = sqrt(-2 * log(r1)) * cos(2 * PI * r2) * stdev;
    double y = sqrt(-2 * log(r1)) * sin(2 * PI * r2) * stdev;
    return {x,y};
}

double sqr(double x){
    return pow(x,2);
}

double cosinus(double x, double y, double z){
    return cos(x*y*z);
}

double Gaussian_pdf(double x, double y, double z, double stddev){
    return pow(1/(stddev*sqrt(2*PI)),3)*exp(-(x*x + y*y + z*z)/(2*pow(stddev,2)));
}

template <typename Func, typename Func2>
double monte_carlo_integration(Func f, Func2 p, std::vector<double> boundaries,int N = 10000);