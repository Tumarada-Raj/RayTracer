#ifndef CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Triangle : public Solid {
public:
    Point vertices[3];
    Point v1, v2, v3;
    Vector edge1, edge2, normal;
    float area;

    Triangle() {}
    Triangle(Point vertices[3], CoordMapper* texMapper, Material* material);
    Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
    float getArea(const Vector& edge1, const Vector& edge2) const;
    Point getBaryCoords(const Point& p) const;
};

}

#endif