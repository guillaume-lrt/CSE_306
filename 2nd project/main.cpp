#include <chrono>

#include "object.hpp"
#include "object.cpp"

using namespace std::chrono;


int main(int argc, char **argv){
    auto start = high_resolution_clock::now();

    // clipping polygon test
    // Polygon subjectPolygon = Polygon({Vector(0.1, 0.9, 0), Vector(0.5, 1, 0), Vector(0.7, 0.8, 0), Vector(0.6, 0.55, 0), Vector(0.9, 0.4, 0), Vector(0.8, 0.2, 0), Vector(0.3, 0.2, 0)});
    // Polygon clipPolygon = Polygon({Vector(0, 1, 0), Vector(1, 0.85, 0), Vector(0.7, 0, 0), Vector(0.2, 0.6, 0)});

    // save_svg("polygons.svg", {subjectPolygon, clipPolygon});

    // Polygon clippedPolygon = clip_poly(subjectPolygon,clipPolygon);

    // save_svg("clipped polygons.svg", {clippedPolygon});

    std::vector<Vector> points = {Vector(0.1, 0.9, 0), Vector(0.5, 1, 0), Vector(0.7, 0.8, 0), Vector(0.6, 0.55, 0), Vector(0.9, 0.4, 0), Vector(0.8, 0.2, 0), Vector(0.3, 0.2, 0)};

    // save_svg("test_points.svg",points,{Polygon(points)});

    std::vector<Polygon> vor = voronoi(points);

    save_svg("Voronoi.svg",points,vor);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    duration = duration / 1000000;
    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    return 0;
}
