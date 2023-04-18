#include <rt/solids/aabox.h>

namespace rt {

AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material)
{
    this->corner1 = min(corner1, corner2);
    this->corner2 = max(corner1, corner2);
    this->material = material;
    if (texMapper != nullptr) this->texMapper = texMapper;
}

BBox AABox::getBounds() const {
    return BBox(corner1, corner2);
}

Solid::Sample AABox::sample() const {
    NOT_IMPLEMENTED;
}

float AABox::getArea() const {
    float xlen = corner2.x - corner1.x;
    float ylen = corner2.y - corner1.y;
    float zlen = corner2.z - corner1.z;
    return 2 * (xlen * ylen + ylen * zlen + zlen * xlen);
}

std::tuple<bool, float, float, Vector> AABox::findRayEntryExit(const Ray& ray) const {
    /* TODO */
    float distance;
    float txNear, txFar, tyNear, tyFar, tzNear, tzFar;
    Vector rayDirInv = Vector(1 / ray.d.x, 1 / ray.d.y, 1 / ray.d.z);

    if (rayDirInv.x >= 0) {
        txNear = (corner1.x - ray.o.x) * rayDirInv.x;
        txFar = (corner2.x - ray.o.x) * rayDirInv.x;
    }
    else {
        txFar = (corner1.x - ray.o.x) * rayDirInv.x;
        txNear = (corner2.x - ray.o.x) * rayDirInv.x;
    }
    if (rayDirInv.y >= 0) {
        tyNear = (corner1.y - ray.o.y) * rayDirInv.y;
        tyFar = (corner2.y - ray.o.y) * rayDirInv.y;
    }
    else {
        tyFar = (corner1.y - ray.o.y) * rayDirInv.y;
        tyNear = (corner2.y - ray.o.y) * rayDirInv.y;
    }
    if (rayDirInv.z >= 0) {
        tzNear = (corner1.z - ray.o.z) * rayDirInv.z;
        tzFar = (corner2.z - ray.o.z) * rayDirInv.z;
    }
    else {
        tzFar = (corner1.z - ray.o.z) * rayDirInv.z;
        tzNear = (corner2.z - ray.o.z) * rayDirInv.z;
    }

    float tmaxNear;
    float tminFar;

    if ((txNear > tyFar) || (tyNear > txFar))
        return std::make_tuple(false, 0.f, 0.f, Vector());
    tmaxNear = std::max(txNear, tyNear);
    tminFar = std::min(txFar, tyFar);
    //tmaxNear = tyNear;
    //tminFar = tyFar;

    if ((tmaxNear > tzFar) || (tzNear > tminFar))
        return std::make_tuple(false, 0.f, 0.f, Vector());
    tmaxNear = std::max(tmaxNear, tzNear);
    tminFar = std::min(tminFar, tzFar);
        //tmaxNear = tzNear;
        //tminFar = tzFar;

    //if((tmaxNear > 0)){
    distance = tmaxNear;
    Vector normal;
    if (tmaxNear - txNear < epsilon) {
        if (rayDirInv.x >= 0)
            normal = Vector(-1.0f, 0, 0);
        else
            normal = Vector(1.0f, 0, 0);
    }
    else if (tmaxNear - tyNear < epsilon) {
        if (rayDirInv.y >= 0)
            normal = Vector(0, -1.0f, 0);
        else
            normal = Vector(0, 1.0f, 0);
    }
    else {
        if (rayDirInv.z >= 0)
            normal = Vector(0, 0, -1.0f);
        else
            normal = Vector(0, 0, 1.0f);
    }
    return std::make_tuple(true, tmaxNear, tminFar, normal);
} 

Intersection AABox::intersect(const Ray& ray, float previousBestDistance) const {
    bool isIntersect;
    float entry, exit;
    Vector normal;

    std::tie(isIntersect, entry, exit, normal) = findRayEntryExit(ray);
    if (isIntersect && (entry > 0) && (entry < previousBestDistance)) {
        Intersection intersection(entry, ray, this, normal, ray.getPoint(entry));
        return intersection;
    }
    else
        return Intersection::failure();
}

}
