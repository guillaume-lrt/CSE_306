#pragma once

#include "vector.hpp"

class Polygon{
    public:
        std::vector<Vector> vertices;
        std::vector<std::pair<Vector,Vector>> edges;

    explicit Polygon(std::vector<Vector> V = {}){
        vertices = V;
        for (auto i = 0; i < vertices.size(); i++){
            // std::pair<Vector, Vector> edge(V[i],V[(i != vertices.size()-1) ? i+1 : 0]);
            std::pair<Vector, Vector> edge(V[i], V[(i != 0) ? i - 1 : vertices.size() - 1]);
            edges.push_back(edge);
        }
    }
};

Vector intersect(Vector prevVertex, Vector curVertex, std::pair<Vector, Vector> clipEddge);

bool inside(Vector P, std::pair<Vector, Vector> clipEdge);

Polygon clip_poly(Polygon subjectPolygon, Polygon clipPolygon);

void save_svg(const std::vector<Polygon> &polygons, std::string filename, std::string fillcol = "none");

void save_svg_animated(const std::vector<Polygon> &polygons, std::string filename, int frameid, int nbframes);