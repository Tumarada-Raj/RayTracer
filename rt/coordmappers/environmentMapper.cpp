#include <rt/coordmappers/environmentMapper.h>

namespace rt {
    //EnvironmentMapper::EnvironmentMapper() {
    //}

    Point EnvironmentMapper::getCoords(const rt::Intersection& hit) const {
        Vector v = hit.ray.d;
        return Point(v.x, v.y, v.z); 
    }
};