#ifndef CG1RAYTRACER_SOLIDS_BOX_HEADER
#define CG1RAYTRACER_SOLIDS_BOX_HEADER

#include <rt/solids/solid.h>
#include <tuple>

namespace rt {

class AABox : public Solid {
public:

    Point corner1;
    Point corner2; 

    AABox() {}
    AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
    std::tuple<bool, float, float, Vector> findRayEntryExit(const Ray& ray) const;
};

}

#endif