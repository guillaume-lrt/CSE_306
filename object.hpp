#pragma once

#include <string>
#include <stdio.h>
#include <algorithm>
#include <vector>

#include "vector.hpp"
#include "Monte_carlo.hpp"

class Camera{
    public:
        Vector center;
        double fov; // field of view
        double f;   // distance screen and center
        double width;
        double height;
        explicit Camera(Vector Q, double alpha, int W, int H)
        {
            center = Q;
            fov = alpha;
            width = W;
            height = H;
        }
        Vector pixel(int x, int y, bool muller = false)
        {
            double f = this->width / (2 * tan(dtr * this->fov / 2));
            if (muller)
            {
                auto [x_shift, y_shift] = boxMuller(1);
                return Vector(this->center[0] + x + 0.5 - this->width / 2 + x_shift,
                            this->center[1] + y + 0.5 - this->height / 2 + y_shift,
                            this->center[2] - f);
            }
            return Vector(this->center[0] + x + 0.5 - this->width / 2,
                        this->center[1] + y + 0.5 - this->height / 2,
                        this->center[2] - f);
        }
};

class Light{
    public:
        Vector origin;
        double intensity;
        explicit Light(Vector C = Vector(0, 0, 0), double I = pow(10, 5))
        {
            origin = C;
            intensity = I;
        }
        // Sphere sphere;
        // double radius;
        // explicit Light(Sphere S = Sphere(), double I = pow(10,5)){ //Vector C, double I, double R){
        //     origin = S.center;
        //     intensity = I;
        //     radius = S.radius;
        //     sphere = S;
        //     sphere.light = true;
        // }
};

class Ray{
    public:
        Vector origin;
        Vector direction;
        explicit Ray(Vector O, Vector u)
        {
            origin = O;
            direction = normalization(u);
        }
};

class Intersection {
    public:
        bool is_intersection;
        Vector position;
        Vector normal;
        double distance;
        int index;
        // bool hollow;
        Intersection(bool b = false, Vector P = Vector(0, 0, 0), Vector N = Vector(0, 0, 0), double d = inf, int i = 0){
            is_intersection = b;
            position = P;
            normal = N;
            distance = d;
            index = i;
            // hollow = h;
            // if (hollow) normal = -N;
        }
};

class Geometry{
    public:
        Vector albedo;
        int index;
        bool mirror = false;
        bool transparent = false;
        double refract_index;
        virtual Intersection intersect(const Ray &r) = 0;
};

class TriangleIndices{
    public:
        TriangleIndices(int vtxi = -1, int vtxj = -1, int vtxk = -1, int ni = -1, int nj = -1, 
                        int nk = -1, int uvi = -1, int uvj = -1, int uvk = -1, int group = -1, 
                        bool added = false) : vtxi(vtxi), vtxj(vtxj), vtxk(vtxk), uvi(uvi),       // same as this->vtxi = vtxi
                        uvj(uvj), uvk(uvk), ni(ni), nj(nj), nk(nk), group(group){};
        int vtxi, vtxj, vtxk; // indices within the vertex coordinates array
        int uvi, uvj, uvk;    // indices within the uv coordinates array
        int ni, nj, nk;       // indices within the normals array
        int group;            // face group
};

class TriangleMesh : public Geometry{
    public:
        ~TriangleMesh() {}
        TriangleMesh(){};

        void readOBJ(const char *obj)    {

            // char matfile[255];
            char grp[255];

            FILE *f;
            f = fopen(obj, "r");
            int curGroup = -1;
            while (!feof(f))
            {
                char line[255];
                if (!fgets(line, 255, f))
                    break;

                std::string linetrim(line);
                linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
                strcpy(line, linetrim.c_str());

                if (line[0] == 'u' && line[1] == 's')
                {
                    sscanf(line, "usemtl %[^\n]\n", grp);
                    curGroup++;
                }

                if (line[0] == 'v' && line[1] == ' ')
                {
                    Vector vec;

                    Vector col;
                    if (sscanf(line, "v %lf %lf %lf %lf %lf %lf\n", &vec[0], &vec[1], &vec[2], &col[0], &col[1], &col[2]) == 6)
                    {
                        col[0] = std::min(1., std::max(0., col[0]));
                        col[1] = std::min(1., std::max(0., col[1]));
                        col[2] = std::min(1., std::max(0., col[2]));

                        vertices.push_back(vec);
                        vertexcolors.push_back(col);
                    }
                    else
                    {
                        sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
                        vertices.push_back(vec);
                    }
                }
                if (line[0] == 'v' && line[1] == 'n')
                {
                    Vector vec;
                    sscanf(line, "vn %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
                    normals.push_back(vec);
                }
                if (line[0] == 'v' && line[1] == 't')
                {
                    Vector vec;
                    sscanf(line, "vt %lf %lf\n", &vec[0], &vec[1]);
                    uvs.push_back(vec);
                }
                if (line[0] == 'f')
                {
                    TriangleIndices t;
                    int i0, i1, i2, i3;
                    int j0, j1, j2, j3;
                    int k0, k1, k2, k3;
                    int nn;
                    t.group = curGroup;

                    char *consumedline = line + 1;
                    int offset;

                    nn = sscanf(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset);
                    if (nn == 9)
                    {
                        if (i0 < 0)
                            t.vtxi = vertices.size() + i0;
                        else
                            t.vtxi = i0 - 1;
                        if (i1 < 0)
                            t.vtxj = vertices.size() + i1;
                        else
                            t.vtxj = i1 - 1;
                        if (i2 < 0)
                            t.vtxk = vertices.size() + i2;
                        else
                            t.vtxk = i2 - 1;
                        if (j0 < 0)
                            t.uvi = uvs.size() + j0;
                        else
                            t.uvi = j0 - 1;
                        if (j1 < 0)
                            t.uvj = uvs.size() + j1;
                        else
                            t.uvj = j1 - 1;
                        if (j2 < 0)
                            t.uvk = uvs.size() + j2;
                        else
                            t.uvk = j2 - 1;
                        if (k0 < 0)
                            t.ni = normals.size() + k0;
                        else
                            t.ni = k0 - 1;
                        if (k1 < 0)
                            t.nj = normals.size() + k1;
                        else
                            t.nj = k1 - 1;
                        if (k2 < 0)
                            t.nk = normals.size() + k2;
                        else
                            t.nk = k2 - 1;
                        indices.push_back(t);
                    }
                    else
                    {
                        nn = sscanf(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset);
                        if (nn == 6)
                        {
                            if (i0 < 0)
                                t.vtxi = vertices.size() + i0;
                            else
                                t.vtxi = i0 - 1;
                            if (i1 < 0)
                                t.vtxj = vertices.size() + i1;
                            else
                                t.vtxj = i1 - 1;
                            if (i2 < 0)
                                t.vtxk = vertices.size() + i2;
                            else
                                t.vtxk = i2 - 1;
                            if (j0 < 0)
                                t.uvi = uvs.size() + j0;
                            else
                                t.uvi = j0 - 1;
                            if (j1 < 0)
                                t.uvj = uvs.size() + j1;
                            else
                                t.uvj = j1 - 1;
                            if (j2 < 0)
                                t.uvk = uvs.size() + j2;
                            else
                                t.uvk = j2 - 1;
                            indices.push_back(t);
                        }
                        else
                        {
                            nn = sscanf(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset);
                            if (nn == 3)
                            {
                                if (i0 < 0)
                                    t.vtxi = vertices.size() + i0;
                                else
                                    t.vtxi = i0 - 1;
                                if (i1 < 0)
                                    t.vtxj = vertices.size() + i1;
                                else
                                    t.vtxj = i1 - 1;
                                if (i2 < 0)
                                    t.vtxk = vertices.size() + i2;
                                else
                                    t.vtxk = i2 - 1;
                                indices.push_back(t);
                            }
                            else
                            {
                                nn = sscanf(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset);
                                if (i0 < 0)
                                    t.vtxi = vertices.size() + i0;
                                else
                                    t.vtxi = i0 - 1;
                                if (i1 < 0)
                                    t.vtxj = vertices.size() + i1;
                                else
                                    t.vtxj = i1 - 1;
                                if (i2 < 0)
                                    t.vtxk = vertices.size() + i2;
                                else
                                    t.vtxk = i2 - 1;
                                if (k0 < 0)
                                    t.ni = normals.size() + k0;
                                else
                                    t.ni = k0 - 1;
                                if (k1 < 0)
                                    t.nj = normals.size() + k1;
                                else
                                    t.nj = k1 - 1;
                                if (k2 < 0)
                                    t.nk = normals.size() + k2;
                                else
                                    t.nk = k2 - 1;
                                indices.push_back(t);
                            }
                        }
                    }

                    consumedline = consumedline + offset;

                    while (true)
                    {
                        if (consumedline[0] == '\n')
                            break;
                        if (consumedline[0] == '\0')
                            break;
                        nn = sscanf(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset);
                        TriangleIndices t2;
                        t2.group = curGroup;
                        if (nn == 3)
                        {
                            if (i0 < 0)
                                t2.vtxi = vertices.size() + i0;
                            else
                                t2.vtxi = i0 - 1;
                            if (i2 < 0)
                                t2.vtxj = vertices.size() + i2;
                            else
                                t2.vtxj = i2 - 1;
                            if (i3 < 0)
                                t2.vtxk = vertices.size() + i3;
                            else
                                t2.vtxk = i3 - 1;
                            if (j0 < 0)
                                t2.uvi = uvs.size() + j0;
                            else
                                t2.uvi = j0 - 1;
                            if (j2 < 0)
                                t2.uvj = uvs.size() + j2;
                            else
                                t2.uvj = j2 - 1;
                            if (j3 < 0)
                                t2.uvk = uvs.size() + j3;
                            else
                                t2.uvk = j3 - 1;
                            if (k0 < 0)
                                t2.ni = normals.size() + k0;
                            else
                                t2.ni = k0 - 1;
                            if (k2 < 0)
                                t2.nj = normals.size() + k2;
                            else
                                t2.nj = k2 - 1;
                            if (k3 < 0)
                                t2.nk = normals.size() + k3;
                            else
                                t2.nk = k3 - 1;
                            indices.push_back(t2);
                            consumedline = consumedline + offset;
                            i2 = i3;
                            j2 = j3;
                            k2 = k3;
                        }
                        else
                        {
                            nn = sscanf(consumedline, "%u/%u%n", &i3, &j3, &offset);
                            if (nn == 2)
                            {
                                if (i0 < 0)
                                    t2.vtxi = vertices.size() + i0;
                                else
                                    t2.vtxi = i0 - 1;
                                if (i2 < 0)
                                    t2.vtxj = vertices.size() + i2;
                                else
                                    t2.vtxj = i2 - 1;
                                if (i3 < 0)
                                    t2.vtxk = vertices.size() + i3;
                                else
                                    t2.vtxk = i3 - 1;
                                if (j0 < 0)
                                    t2.uvi = uvs.size() + j0;
                                else
                                    t2.uvi = j0 - 1;
                                if (j2 < 0)
                                    t2.uvj = uvs.size() + j2;
                                else
                                    t2.uvj = j2 - 1;
                                if (j3 < 0)
                                    t2.uvk = uvs.size() + j3;
                                else
                                    t2.uvk = j3 - 1;
                                consumedline = consumedline + offset;
                                i2 = i3;
                                j2 = j3;
                                indices.push_back(t2);
                            }
                            else
                            {
                                nn = sscanf(consumedline, "%u//%u%n", &i3, &k3, &offset);
                                if (nn == 2)
                                {
                                    if (i0 < 0)
                                        t2.vtxi = vertices.size() + i0;
                                    else
                                        t2.vtxi = i0 - 1;
                                    if (i2 < 0)
                                        t2.vtxj = vertices.size() + i2;
                                    else
                                        t2.vtxj = i2 - 1;
                                    if (i3 < 0)
                                        t2.vtxk = vertices.size() + i3;
                                    else
                                        t2.vtxk = i3 - 1;
                                    if (k0 < 0)
                                        t2.ni = normals.size() + k0;
                                    else
                                        t2.ni = k0 - 1;
                                    if (k2 < 0)
                                        t2.nj = normals.size() + k2;
                                    else
                                        t2.nj = k2 - 1;
                                    if (k3 < 0)
                                        t2.nk = normals.size() + k3;
                                    else
                                        t2.nk = k3 - 1;
                                    consumedline = consumedline + offset;
                                    i2 = i3;
                                    k2 = k3;
                                    indices.push_back(t2);
                                }
                                else
                                {
                                    nn = sscanf(consumedline, "%u%n", &i3, &offset);
                                    if (nn == 1)
                                    {
                                        if (i0 < 0)
                                            t2.vtxi = vertices.size() + i0;
                                        else
                                            t2.vtxi = i0 - 1;
                                        if (i2 < 0)
                                            t2.vtxj = vertices.size() + i2;
                                        else
                                            t2.vtxj = i2 - 1;
                                        if (i3 < 0)
                                            t2.vtxk = vertices.size() + i3;
                                        else
                                            t2.vtxk = i3 - 1;
                                        consumedline = consumedline + offset;
                                        i2 = i3;
                                        indices.push_back(t2);
                                    }
                                    else
                                    {
                                        consumedline = consumedline + 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            fclose(f);
        }

        std::vector<TriangleIndices> indices;       // triangles => composed of the indices for vertex, uv and normals
        std::vector<Vector> vertices;
        std::vector<Vector> normals;
        std::vector<Vector> uvs;
        std::vector<Vector> vertexcolors;

        Intersection intersect(const Ray &r);
};

class Sphere : public Geometry {
    public:
        Vector center;
        double radius;
        // bool light = false;
        Intersection intersect(const Ray &r);
        bool hollow;

        explicit Sphere(Vector C = Vector(0,0,0), double R = 0,Vector A =Vector(0,0,0),std::string surface = "diffuse",double n = 0, bool h = false){
            center = C;
            radius = R;
            albedo = A;
            refract_index = n;
            if (surface == "mirror") mirror = true;
            if (surface == "transparent") transparent = true;
            hollow = h;
        }
};