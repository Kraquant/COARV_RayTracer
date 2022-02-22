#include <sphere.h>

bool sphere::hit(const ray&r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot (oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant<0) return false;
    auto sqrtd = sqrt(discriminant);

    //Find the nearest root that lies in the acceptable range
    auto root = (-half_b - sqrtd) / a; //Etude de la premiere racine
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root ) {
            return false; //Aucune des deux racines n'est dans une range acceptable
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p- this->center) / radius;
    return true;
}


//Première version du code avec non simplifié
// double hit_sphereV1(const point3& center, double radius, const ray& r) {
//     //Si le rayon est donne par P(t) = A + t*B
//     //On cherche a resoudre l'equation ou il existe t tel que:
//     // norm(P(t) - C) <= r
//     // soit (P(t)-C).(P(t)-C) <= r
//     // soit un equation du second degré
    
//     vec3 oc = r.origin() - center;

//     auto a = dot(r.direction(), r.direction());
//     auto b = 2*dot(r.direction(), oc);
//     auto c = oc.length_squared() - radius*radius;

//     auto discriminant = b*b - 4*a*c; //Une ou deux solution: le rayon frole ou traverse la boule
//     if (discriminant < 0 ) {
//         return -1.0;
//     } else {
//         return (-b-sqrt(discriminant))/(2*a); //On retourne une des deux solutions, le plus petit t
//     }    
// }

//Deuxième version du code sachant qu'on peut écrire B=2*h et en remplacant par les normes au carré
// double hit_sphere(const point3& center, double radius, const ray& r) {
//     //Si le rayon est donne par P(t) = A + t*B
//     //On cherche a resoudre l'equation ou il existe t tel que:
//     // norm(P(t) - C) <= r
//     // soit (P(t)-C).(P(t)-C) <= r
//     // soit un equation du second degré
    
//     vec3 oc = r.origin() - center;

//     auto a = r.direction().length_squared();
//     auto half_b = dot(r.direction(), oc);
//     auto c = oc.length_squared() - radius*radius;

//     auto discriminant =  half_b*half_b - a*c; //Une ou deux solution: le rayon frole ou traverse la boule
//     if (discriminant < 0 ) {
//         return -1.0;
//     } else {
//         return (-half_b-sqrt(discriminant))/a; //On retourne une des deux solutions, le plus petit t
//     }    
// }