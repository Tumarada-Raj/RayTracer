#include <rt/solids/infiniteplane.h>

namespace rt {

InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material)
{
    this->origin = origin;
    this->normal = normal;
    this->material = material;
    if (texMapper != nullptr) this->material = material;
}

BBox InfinitePlane::getBounds() const {
    return BBox::full();
}

Intersection InfinitePlane::intersect(const Ray& ray, float previousBestDistance) const {

    float denom = dot(ray.d, normal);

    if (denom < epsilon && denom> -epsilon)
        return Intersection::failure();

    float distance = dot(origin - ray.o, normal) / denom;
    if ((distance >= 0) && (distance < previousBestDistance)) {
        Intersection intersection(distance, ray, this, normal, ray.getPoint(distance));
        return intersection;
    }
    else
        return Intersection::failure();
}

Solid::Sample InfinitePlane::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float InfinitePlane::getArea() const {
    /* TODO */ NOT_IMPLEMENTED;
}

}
