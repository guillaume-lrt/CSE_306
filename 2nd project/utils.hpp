#pragma once

#include <limits>
#include <random>

#define PI 3.14159265
#define rtd double(180 / PI) // radiantodegree: radian * rtd = degree
#define dtr double(PI / 180)

double inf = std::numeric_limits<double>::infinity();

// static std::default_random_engine engine_scene[8];
static std::default_random_engine engine_scene(time(0));
static std::uniform_real_distribution<double> uniform_scene(0, 1);