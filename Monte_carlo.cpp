#include <iostream>
#include <random>
#define PI 3.14159265

static std::default_random_engine engine(10); // random seed = 10
static std::uniform_real_distribution<double> uniform(0, 1);


void boxMuller(double stdev, double &x, double &y){
    double r1 = uniform(engine);
    double r2 = uniform(engine);
    x = sqrt(-2 * log(r1)) * cos(2 * PI * r2) * stdev;
    y = sqrt(-2 * log(r1)) * sin(2 * PI * r2) * stdev;
}

double sqr(double x){
    return pow(x,2);
}

double cosinus(double x){
    return cos(x);
}

double Gaussian_pdf(double x, double y, double z, double stddev){
    return pow(1/(stddev*sqrt(2*PI)),3)*exp(-(x*x + y*y + z*z)/(2*pow(stddev,2)));
}

template <typename Func>
double monte_carlo_integration(Func f, Func p, std::vector<double> boundaries,int N = 100){
    double a = boundaries[0];
    double b = boundaries[1];
    std::uniform_real_distribution<double> uniform_int(a, b);
    double res = 0;
    for (int i = 0; i < N; i++){
        double x1 = uniform_int(engine);
    }
    return f(3);
}

int main(){
    auto t = monte_carlo_integration(cosinus);
    std::cout << t;
    return 0;
}