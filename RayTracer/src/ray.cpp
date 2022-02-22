#include <ray.h>

ray::ray() {}
ray::ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

point3 ray::at(double t) const {
    return orig + t*dir;
}

point3 ray::origin() const {
    return this->orig;
}

vec3 ray::direction() const {
    return this->dir;
}