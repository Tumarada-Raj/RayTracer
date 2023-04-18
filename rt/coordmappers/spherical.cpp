#include <rt/coordmappers/spherical.h>

namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
{
    this->origin = origin;
    this->zenith = zenith;
    this->azimuthRef = azimuthRef;
    this->scaleX = azimuthRef.length();
    this->scaleY = zenith.length();
}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    Vector hitP = (hit.local() - origin).normalize();
    float phi = acos(dot(zenith.normalize(), hitP));
    float v = phi / (pi * scaleY);

    Vector polarHit = (hitP - dot(zenith, hitP) * zenith) / (scaleY * scaleY);

    float theta = acos(dot(azimuthRef.normalize(), hitP) / sin(phi));
    float u;
    Vector perpPolar = (azimuthRef - dot(azimuthRef, zenith.normalize()) * zenith);
    dot(cross(zenith, perpPolar), polarHit) > 0 ? u = (2 * pi - theta) / (2 * pi * scaleX) : u = theta / (2 * pi * scaleX);
    return Point(u, v, 0);
}

}