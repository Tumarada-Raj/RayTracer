#ifndef CG1RAYTRACER_INTERSECTION_HEADER
#define CG1RAYTRACER_INTERSECTION_HEADER

#include <core/scalar.h>
#include <core/vector.h>
#include <rt/ray.h>

namespace rt {

class Solid;

class Intersection {
public:
    Ray ray;
    const Solid* solid;
    float distance;
    Vector normalVec;
    Point localP;
    bool isIntersect; //Added new variable to check for intersection, By default there is no intersection.

    Intersection() { isIntersect = false; }
    static Intersection failure();
    Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& uv);

    Point hitPoint() const;
    Vector normal() const;
    Point local() const;
    Vector setNormal(const Vector n);

    operator bool() const; //this allows intersection object to be put directly in conditional statements. Becomes true iff there is an intersection
};

}

#endif