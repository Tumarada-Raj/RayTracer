#include <rt/coordmappers/tmapper.h>
#include <core/float4.h>

namespace rt {

TriangleMapper::TriangleMapper(Point ntv[3])
{
    this->tv0 = ntv[0];
    this->tv1 = ntv[1];
    this->tv2 = ntv[2];
}

TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2)
{
    this->tv0 = tv0;
    this->tv1 = tv1;
    this->tv2 = tv2;
}

Point TriangleMapper::getCoords(const Intersection& hit) const {
    //Point local = hit.local();
    //Point t = local.x * tv0 + local.y * tv1 + local.z * tv2;

    Float4 p = Float4(hit.local().x * tv0) + Float4(hit.local().y * tv1) + Float4(hit.local().z * tv2);
    return Point(p.x, p.y, p.z);
 
    //return t;
}

}