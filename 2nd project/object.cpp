#include "object.hpp"

#include <map>
#include <algorithm>

Vector intersect(Vector prevVertex, Vector curVertex, std::pair<Vector,Vector> clipEdge){
    // Sutherland-Hodgman clipping algorythm
    // return the intersection between two segments
    Vector u = clipEdge.first;
    Vector v = clipEdge.second;
    Vector N = Vector(v[1]-u[1],u[0]-v[0],0);       // normal to line (u,v)
    double t = dot(u-prevVertex,N)/dot(curVertex-prevVertex,N);
    if (t < 0 || t > 1){    // no intersection
        // std::cout << "t < 0 or t > 1\n";
        return Vector(inf,inf,inf);
    }
    return prevVertex + t*(curVertex-prevVertex);
};

Vector intersect_voronoi(Vector &A, Vector &B, Vector &P, Vector &Pj){
    Vector M = (P + Pj)/2;
    double t = dot(M-A,P-Pj)/dot(B-A,P-Pj);
    return A + t*(B-A);
}

bool inside(Vector P, std::pair<Vector,Vector> clipEdge){
    Vector u = clipEdge.first;
    Vector v = clipEdge.second;
    Vector N = Vector(v[1] - u[1], u[0] - v[0],0);
    return dot(P-u,N) <= 0 ? true : false;
}

bool inside(Vector X, Vector P, Vector Pj){
    // return true if X is inside the clipe edge
    Vector M = (P+Pj)/2;
    return dot(X-M,Pj-P)<0;
}

double max_dist(Polygon polygon,Vector P){
    // square distance betwee P and the furthest point of the current voronoi cell
    double dist = 0;
    for (auto &vert: polygon.vertices){
        dist = std::max(dist, distance_square(vert, P));
    }
    return dist;
}

Polygon clip_poly(Polygon subjectPolygon, Polygon clipPolygon){
    // reduce the subject polygone by taking the intersection with clipPolygon
    Polygon outPolygon;     // return polygon
    for(auto &clipEdge : clipPolygon.edges){
        outPolygon = Polygon();
        for (int i = 0; i < subjectPolygon.vertices.size(); i++){
            Vector curVertex = subjectPolygon.vertices[i];
            Vector prevVertex = subjectPolygon.vertices[(i>0)?(i-1):subjectPolygon.vertices.size()-1];
            Vector intersection = intersect(prevVertex,curVertex,clipEdge);
            if (inside(curVertex,clipEdge)){
                if (!inside(prevVertex,clipEdge)) outPolygon.vertices.push_back(intersection);
                outPolygon.vertices.push_back(curVertex);
            }
            else if (inside(prevVertex,clipEdge)) outPolygon.vertices.push_back(intersection);
        }
        subjectPolygon = outPolygon;
    }
    return outPolygon;
};

// std::vector<Polygon> voronoi(std::vector<Vector> points,Polygon space){
//     // small issue
//      std::vector<Polygon> res;
//     Polygon outPolygon;
//     for(auto &curPoint: points){
//         std::map<double,Vector> otherPoints;    // list of all other points sorted by their distance from curPoint
//         for(auto &tempPoint: points){
//             if (tempPoint != curPoint){
//                 otherPoints[distance_square(curPoint,tempPoint)] = tempPoint;
//             }
//         }
//         outPolygon = space;
//         for(auto &it: otherPoints){
//             if (it.first > 2*max_dist(outPolygon,curPoint)) break;      // this point (and thus also those further away) don't affect the voronoi cell of curPoint
//             auto tempPoint = it.second;
//             Vector M = (tempPoint + curPoint)/2;
//             size_t n = outPolygon.vertices.size();
//             Polygon tempPolygon = Polygon();
//             for(int i = 0; i < n; i++){
//                 Vector curVertex = outPolygon.vertices[i];
//                 Vector prevVertex = outPolygon.vertices[(i>0)?(i-1):n-1];
//                 Vector intersection = intersect_voronoi(prevVertex,curVertex,curPoint,tempPoint);
//                 if(inside(curVertex,curPoint,tempPoint)){
//                     if (!inside(prevVertex, curPoint,tempPoint))
//                         tempPolygon.vertices.push_back(intersection);
//                     tempPolygon.vertices.push_back(curVertex);
//                 }
//                 else if (inside(prevVertex, curPoint,tempPoint))
//                     tempPolygon.vertices.push_back(intersection);
//             }
//             outPolygon = tempPolygon;
//         }
//         res.push_back(outPolygon);
//     }
//     return res;
// }

std::vector<Polygon> voronoi(std::vector<Vector> points,Polygon space){
    std::vector<Polygon> res;
    Polygon outPolygon;
    for(auto &curPoint: points){
        outPolygon = space;
        for(auto &tempPoint: points){
            if (tempPoint != curPoint){
                Vector M = (tempPoint + curPoint)/2;
                size_t n = outPolygon.vertices.size();
                Polygon tempPolygon = Polygon();
                for(int i = 0; i < n; i++){
                    Vector curVertex = outPolygon.vertices[i];
                    Vector prevVertex = outPolygon.vertices[(i>0)?(i-1):n-1];
                    Vector intersection = intersect_voronoi(prevVertex,curVertex,curPoint,tempPoint);
                    if(inside(curVertex,curPoint,tempPoint)){
                        if (!inside(prevVertex, curPoint,tempPoint))
                            tempPolygon.vertices.push_back(intersection);
                        tempPolygon.vertices.push_back(curVertex);
                    }
                    else if (inside(prevVertex, curPoint,tempPoint))
                        tempPolygon.vertices.push_back(intersection);
                }
                outPolygon = tempPolygon;
            }
        }
        res.push_back(outPolygon);
    }
    return res;
}

// saves a static svg file. The polygon vertices are supposed to be in the range [0..1], and a canvas of size 1000x1000 is created
void save_svg(std::string filename, const std::vector<Polygon> &polygons, const std::vector<Vector> &vectors, std::string fillcol)
{
    FILE *f = fopen(filename.c_str(), "w+");
    fprintf(f, "<svg xmlns = \"http://www.w3.org/2000/svg\" width = \"1000\" height = \"1000\">\n");
    if (!polygons.empty()){
        for (int i = 0; i < polygons.size(); i++){
            fprintf(f, "<g>\n");
            fprintf(f, "<polygon points = \"");
            for (int j = 0; j < polygons[i].vertices.size(); j++){
                fprintf(f, "%3.3f, %3.3f ", (polygons[i].vertices[j][0] * 1000), (1000 - polygons[i].vertices[j][1] * 1000));
            }
            fprintf(f, "\"\nfill = \"%s\" stroke = \"black\"/>\n", fillcol.c_str());
            fprintf(f, "</g>\n");
        }
    }
    if (!vectors.empty()){
        for (auto &vect : vectors){
            fprintf(f, "<g>\n");
            fprintf(f, "<circle cx = \"%3.3f\" cy = \"%3.3f\" r=\"5\"/>", vect[0]*1000, 1000-vect[1]*1000);
            fprintf(f, "</g>\n");
        }
    }
    fprintf(f, "</svg>\n");
    fclose(f);
}

void save_svg(std::string filename, const std::vector<Vector> &vectors, const std::vector<Polygon> &polygons, std::string fillcol)
{
    FILE *f = fopen(filename.c_str(), "w+");
    fprintf(f, "<svg xmlns = \"http://www.w3.org/2000/svg\" width = \"1000\" height = \"1000\">\n");
    for (int i = 0; i < polygons.size(); i++){
        fprintf(f, "<g>\n");
        fprintf(f, "<polygon points = \"");
        for (int j = 0; j < polygons[i].vertices.size(); j++){
            fprintf(f, "%3.3f, %3.3f ", (polygons[i].vertices[j][0] * 1000), (1000 - polygons[i].vertices[j][1] * 1000));
        }
        fprintf(f, "\"\nfill = \"%s\" stroke = \"black\"/>\n", fillcol.c_str());
        fprintf(f, "</g>\n");
    }
    for (auto &vect : vectors){
        fprintf(f, "<g>\n");
        fprintf(f, "<circle cx = \"%3.3f\" cy = \"%3.3f\" r=\"5\"/>", vect[0] * 1000, 1000 - vect[1] * 1000);
        fprintf(f, "</g>\n");
    }
    fprintf(f, "</svg>\n");
    fclose(f);
}

// Adds one frame of an animated svg file. frameid is the frame number (between 0 and nbframes-1).
// polygons is a list of polygons, describing the current frame.
// The polygon vertices are supposed to be in the range [0..1], and a canvas of size 1000x1000 is created
void save_svg_animated(const std::vector<Polygon> &polygons, std::string filename, int frameid, int nbframes){
    FILE *f;
    if (frameid == 0){
        f = fopen(filename.c_str(), "w+");
        fprintf(f, "<svg xmlns = \"http://www.w3.org/2000/svg\" width = \"1000\" height = \"1000\">\n");
        fprintf(f, "<g>\n");
    }
    else{
        f = fopen(filename.c_str(), "a+");
    }
    fprintf(f, "<g>\n");
    for (int i = 0; i < polygons.size(); i++){
        fprintf(f, "<polygon points = \"");
        for (int j = 0; j < polygons[i].vertices.size(); j++){
            fprintf(f, "%3.3f, %3.3f ", (polygons[i].vertices[j][0] * 1000), (1000 - polygons[i].vertices[j][1] * 1000));
        }
        fprintf(f, "\"\nfill = \"none\" stroke = \"black\"/>\n");
    }
    fprintf(f, "<animate\n");
    fprintf(f, "    id = \"frame%u\"\n", frameid);
    fprintf(f, "    attributeName = \"display\"\n");
    fprintf(f, "    values = \"");
    for (int j = 0; j < nbframes; j++){
        if (frameid == j){
            fprintf(f, "inline");
        }
        else{
            fprintf(f, "none");
        }
        fprintf(f, ";");
    }
    fprintf(f, "none\"\n    keyTimes = \"");
    for (int j = 0; j < nbframes; j++){
        fprintf(f, "%2.3f", j / (double)(nbframes));
        fprintf(f, ";");
    }
    fprintf(f, "1\"\n   dur = \"5s\"\n");
    fprintf(f, "    begin = \"0s\"\n");
    fprintf(f, "    repeatCount = \"indefinite\"/>\n");
    fprintf(f, "</g>\n");
    if (frameid == nbframes - 1){
        fprintf(f, "</g>\n");
        fprintf(f, "</svg>\n");
    }
    fclose(f);
}