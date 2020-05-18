#include "Monte_carlo.hpp"

template <typename Func, typename Func2>
double monte_carlo_integration(Func &f, Func2 &p, std::vector<double> boundaries,int N){
    double a = boundaries[0];
    double b = boundaries[1];
    double res = 0;
    double stddev = 1;
    for (int i = 0; i < N; i++){
        auto [x,y] = boxMuller(stddev);
        auto [z,w] = boxMuller(stddev);
        if ((x - b) * (x - a) <= 0 && (y - b) * (y - a) <= 0 && (z - b) * (z - a) <= 0){
            res += (f(x,y,z) / p(x, y, z, stddev));
        }
    }
    res /= N;
    return res;
}

int main(){
    std::vector<double> bound = {-PI/2,PI/2};
    auto t = monte_carlo_integration(cosinus,Gaussian_pdf,bound);   // exercise
    std::cout << t << std::endl;
    return 0;
}