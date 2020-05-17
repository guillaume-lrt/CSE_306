#include "object.hpp"

Vector intersect(Vector prevVertex, Vector curVertex, std::pair<Vector,Vector> clipEdge){
    Vector u = clipEdge.first;
    Vector v = clipEdge.second;
    Vector N = Vector(v[1]-u[1],u[0]-v[0],0);       // normal to line (u,v)
    double t = dot(u-prevVertex,N)/dot(curVertex-prevVertex,N);
    if (t < 0 || t > 1){    // no intersection
        std::cout << "t < 0 or t > 1\n";
        // return Vector(inf,inf,inf);
    }
    return prevVertex + t*(curVertex-prevVertex);
};

bool inside(Vector P, std::pair<Vector,Vector> clipEdge){
    Vector u = clipEdge.first;
    Vector v = clipEdge.second;
    Vector N = Vector(v[1] - u[1], u[0] - v[0],0);
    return dot(P-u,N) <= 0 ? true : false;
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

// saves a static svg file. The polygon vertices are supposed to be in the range [0..1], and a canvas of size 1000x1000 is created
void save_svg(const std::vector<Polygon> &polygons, std::string filename, std::string fillcol){
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