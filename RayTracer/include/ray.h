#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    
    public:
    point3 orig;
    vec3 dir;
    public:
    ray ();
    ray(const point3& origin, const vec3& direction);

    point3 at(double t) const;

};

#endif