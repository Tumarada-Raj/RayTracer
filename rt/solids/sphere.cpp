#include <rt/solids/sphere.h>
//#include<corecrt_math_defines.h>

namespace rt {

Sphere::Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material)
{
    this->center = center;
    this->radius = radius;
    if (texMapper != nullptr) this->texMapper = texMapper;
    this->material = material;
}

BBox Sphere::getBounds() const {
    Point min(center.x - radius, center.y - radius, center.z - radius);
    Point max(center.x + radius, center.y + radius, center.z + radius);
    return BBox(min, max);
}

Intersection Sphere::intersect(const Ray& ray, float previousBestDistance) const {
    float distance;
    Vector oc = ray.o - center;
    float a = dot(ray.d, ray.d);
    float b = 2.0f * dot(oc, ray.d);
    float c = dot(oc, oc) - (radius * radius);

    //Peter Shirley textbook pg76&77.
    float discriminant = (b * b) - (4 * a * c);
    if (discriminant >= 0.0f) {
        float t0 = (-b + sqrt(discriminant)) / (2 * a);
        float t1 = (-b - sqrt(discriminant)) / (2 * a);

        //Checking for all discriminant(delta) cases.
        if (t1 < t0)
            std::swap(t0, t1);
        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead 
            if (t0 < 0)
                return Intersection::failure(); // both t0 and t1 are negative 
        }
        distance = t0;
    }
    else
        return Intersection::failure();

    if (distance < previousBestDistance) {
        Vector normal = (ray.getPoint(distance) - center).normalize();
        Intersection intersection(distance, ray, this, normal, ray.getPoint(distance));
        return intersection;
    }
    else
        return Intersection::failure();
}


Solid::Sample Sphere::sample() const {
	NOT_IMPLEMENTED;
}

float Sphere::getArea() const {
    return 4 * pi * radius * radius;
}
}
