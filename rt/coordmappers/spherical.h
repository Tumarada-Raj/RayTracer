#ifndef CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER

#include <rt/coordmappers/coordmapper.h>
#include <core/matrix.h>

namespace rt {

class Vector;

class SphericalCoordMapper : public CoordMapper {
public:
    Point origin;
    Vector zenith, azimuthRef;
    float radius, scaleX, scaleY;
    SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef);
    virtual Point getCoords(const Intersection& hit) const;
};

}

#endif