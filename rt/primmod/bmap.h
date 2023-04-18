#ifndef CG1RAYTRACER_PRIMMOD_BMAP_HEADER
#define CG1RAYTRACER_PRIMMOD_BMAP_HEADER

#include <rt/primitive.h>

namespace rt {

class Triangle;
class Texture;

class BumpMapper : public Primitive {
public:
    Triangle* base; Texture* bumpmap; float vscale;
    CoordMapper* cmapper;
    Vector O_u, O_v;
    BumpMapper(Triangle* base, Texture* bumpmap, const Point& bv1, const Point& bv2, const Point& bv3, float vscale);
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);
};

}

#endif