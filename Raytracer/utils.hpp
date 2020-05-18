#pragma once

#include <limits>

#define PI 3.14159265
#define rtd double(180 / PI) // radiantodegree: radian * rtd = degree
#define dtr double(PI / 180)

double inf = std::numeric_limits<double>::infinity();