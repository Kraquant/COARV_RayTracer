#ifndef MATERIAL_H
#define MATERIAL_H

#include <rtweekend.h>
#include <hittable.h>

struct hit_record;

class material {
    public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
    public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
        vec3 scatter_direction = rec.normal + vec3::random_unit_vector();

        //Catch degenerate scatter direction
        if (scatter_direction.near_zero()) scatter_direction = rec.normal;
        
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

    public:
    color albedo;
};

class metal : public material {
    public:
    color albedo;
    double fuzz;

    public:
    metal(const color& a, double f) : albedo(a), fuzz(f<1 ? f : 1) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflected = vec3::reflect(unit_vector(r_in.direction()), rec.normal);
        scattered  = ray(rec.p, reflected + fuzz*vec3::random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

class dielectric : public material {
    public:
    double ir; //Index of refraction

    public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        vec3 unit_direction = unit_vector(r_in.direction());
        vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, refracted);
        return true;

    }

};

#endif