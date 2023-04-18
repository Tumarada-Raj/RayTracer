#include <rt/coordmappers/world.h>

namespace rt {

Point WorldMapper::getCoords(const Intersection& hit) const {
    Point worldHitCoord = hit.hitPoint();
    Point worldHitCoordScaled;
    worldHitCoordScaled.x = worldHitCoord.x * scale.x;
    worldHitCoordScaled.y = worldHitCoord.y * scale.y;
    worldHitCoordScaled.z = worldHitCoord.z * scale.z;
    return worldHitCoordScaled;
}

WorldMapper::WorldMapper()
{
    this->scale = Vector::rep(1.0f);
}

WorldMapper::WorldMapper(const Vector& scale)
{
    this->scale = scale;
}

}