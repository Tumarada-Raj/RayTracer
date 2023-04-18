#ifndef CG1RAYTRACER_COORDMAPPERS_PLANE_HEADER
#define CG1RAYTRACER_COORDMAPPERS_PLANE_HEADER

#include <rt/coordmappers/coordmapper.h>
#include <core/matrix.h> 

namespace rt {

class Intersection;
class Vector;

class PlaneCoordMapper : public CoordMapper{
public:
    Vector e1, e2, normal;
    Point origin;
    Matrix m;
    PlaneCoordMapper(const Vector& e1, const Vector& e2);
    virtual Point getCoords(const Intersection& hit) const;
};

}

#endif