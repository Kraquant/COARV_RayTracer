#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <fstream>

#include <rtweekend.h>
#include <color.h>
#include <sphere.h>
#include <hittable_list.h>
#include <camera.h>
#include <material.h>
#include <string>

color ray_color(const ray& r, const hittable& world, int depth);
void askParam(int&param, std::string paramName, const int& default);
void askForParams(std::string& name, int& width, int& samples, int& depth);

int main() {
    int image_width, samples_per_pixel, max_depth;
    std::string render_name;
    askForParams(render_name, image_width, samples_per_pixel, max_depth);

    //Image
    const double aspect_ratio = 3.0 / 2.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    
    //World
    auto world = random_scene();

    //Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    //Render
    std::ofstream render_image(render_name);

    render_image << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j  = image_height-1; j>=0; --j){
        system("CLS");
        std::cout << "##########     " << render_name << "     ##########" << std::endl << std::endl;
        std::cout << "Rendering parameters:" << std::endl;
        std::cout << "Image width: " << image_width << " | Image Height: " << image_height << " | Aspect ratio: " << aspect_ratio << std::endl;
        std::cout << "Samples per pixel: " << samples_per_pixel << " | Max depth: " << max_depth << std::endl << std::endl;
        std::cout << "Rendering image..." << std::endl;
        std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
        

        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(render_image, pixel_color, samples_per_pixel);
        }
    }

    render_image.close();
    std::cerr << "\nDone.\n";
    //system("pause");
    return 0;
}

color ray_color(const ray& r, const hittable& world, int depth) { 
    
    hit_record rec;

    //If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0) return color(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }

    //Gradient du ciel si on ne touche rien
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

void askParam(int&param, std::string paramName, const int& default)
{
    bool validAnswer = false;  
    int value;  
    std::string input;
    while (!validAnswer)
    {
        validAnswer = true;
        std::cout << "Enter " << paramName << ". Default: " << default << "[d for default]" << std::endl;
        std::cin >> input;
        if (input == "d"){
            std::cout << "Using default value: " << default << std::endl;
            value = default;
        } else {
            try
            {
                value = std::stoi(input);
                std::cout << "The value is: " << value << " (and the string was \"" << input << "\")\n";
            } catch(std::invalid_argument const&) {
                validAnswer = false;
                std::cout << "The provided value is not an integer\n";
            } catch(std::out_of_range const&) {
                validAnswer = false;
                std::cout << "The provided value is out of range\n";
            }
        }
    }
    param = value;
}

void askForParams(std::string& name, int& width, int& samples, int& depth) {
    //Setting up the name of the file
    const std::string default_name = "myFinalRender";
    std::string nameEntry;
    std::cout << "Please enter the name of the file: (default is " << default_name << ") [d for default]" << std::endl;
    std::cin >> nameEntry;
    if (nameEntry == "d"){
        name = default_name + ".ppm";
    }
    else {
        name = nameEntry + ".ppm";
    }

    //Setting up other parameters
    const int default_image_width = 1200;
    const int default_samples_per_pixel = 500;
    const int default_max_depth = 50;

    int default_ImageProperties = 2;
    std::string ans;

    std::cout << "Do you want to change the image settings ? [y/n]" << std::endl;
    std::cin >> ans;
    while(default_ImageProperties >= 2) {
        if (ans == "y") {
            default_ImageProperties = 1;
        }
        else if (ans == "n") {
            default_ImageProperties = 0;
        }
        else {
            system("CLS");
            std::cout << "Invalid entry please" << std::endl;
            std::cout << "Do you want to change the image settings ? [y/n]" << std::endl;
            std::cin >> ans;
        }
    }
    system("CLS");
    if (default_ImageProperties > 0){
        askParam(width, "image width", default_image_width);
        Sleep(2000);
        system("CLS");
        askParam(samples, "samples per pixel", default_samples_per_pixel);
        Sleep(2000);
        system("CLS");
        askParam(depth, "max depth", default_max_depth);
        Sleep(2000);
        system("CLS");
    } else {
        width = default_image_width;
        samples = default_samples_per_pixel;
        depth = default_max_depth;
        std::cout << "Using default parameters:" << std::endl;
        std::cout << "Image width: " << default_image_width << std::endl;
        std::cout << "Samples per pixel: " << default_samples_per_pixel << std::endl;
        std::cout << "Max depth: " << default_max_depth << std::endl;
        Sleep(2000);
        system("CLS");
    }


}
