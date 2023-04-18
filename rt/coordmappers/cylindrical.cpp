#include <rt/coordmappers/cylindrical.h>

namespace rt {

CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis)
{
    this->origin = origin;
    this->longitudinalAxis = longitudinalAxis;
    this->polarAxis = polarAxis;
    this->scaleY = longitudinalAxis.length();
    this->scaleX = polarAxis.length();
}

Point CylindricalCoordMapper::getCoords(const Intersection& hit) const {

    Vector hitP = hit.local() - origin;
    
    //Projection of hitP onto lA gives y-component, divide by lA.length as this is height of cylinder
    float v = dot(longitudinalAxis, hitP) / (scaleY* scaleY);

    //Project hitP onto the plane perpendicular to lA, & containing origin
    Vector polarHit = hitP - v * longitudinalAxis;

    //phi in the plane between new polar axis and projector vector
    float ud = acos(dot(polarAxis.normalize(), polarHit.normalize()));
    float u;
    dot(polarHit, cross(longitudinalAxis, polarAxis).normalize()) > 0 ? u = (1/scaleX) - (ud/(2*pi*scaleX)) : u = ud/(2*pi*scaleX);
    return Point(u, v, 0);
}

}