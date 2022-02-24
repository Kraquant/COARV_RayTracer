#ifndef CAMERA_H
#define CAMERA_H

#include <rtweekend.h>

class camera {
    private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

    public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio);
    ray get_ray(double s, double t) const;
};

#endif