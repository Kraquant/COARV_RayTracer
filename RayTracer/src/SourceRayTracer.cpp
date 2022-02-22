#include <iostream>
#include <fstream>

#include <vec3.h>
#include <color.h>
#include <ray.h>

bool hit_sphere(const point3& center, double radius, const ray& r) {
    //Si le rayon est donne par P(t) = A + t*B
    //On cherche a resoudre l'equation ou il existe t tel que:
    // norm(P(t) - C) <= r
    // soit (P(t)-C).(P(t)-C) <= r
    // soit un equation du second degré
    
    vec3 oc = r.origin() - center;

    auto a = dot(r.direction(), r.direction());
    auto b = 2*dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = b*b - 4*a*c;
    return discriminant > 0; //Une ou deux solution: le rayon frole ou traverse la boule
}

color ray_color(const ray& r) { //Returns the color of the background (a gradient)

    //Ajout de l'intersection à la boule:
    if (hit_sphere(point3(0,0,-1), 0.5, r)) return color(1, 0, 0);

    //Gradient du ciel
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {


    //Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    
    //Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0, focal_length);

    //Render
    std::ofstream render_image("myRender.ppm");

    render_image << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j  = image_height-1; j>=0; j--){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);

            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(render_image, pixel_color);
        }
    }

    render_image.close();
    std::cerr << "\nDone.\n";
    system("pause");
    return 0;
}