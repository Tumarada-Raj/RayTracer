#include <rt/solids/disc.h>
//#include<corecrt_math_defines.h>
#include <core/random.h>

namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material)
{
    this->center = center;
    this->radius = radius;
    this->normal = normal;
    this->material = material;
    if (texMapper != nullptr) this->texMapper = texMapper;

}

BBox Disc::getBounds() const {
    float radX = radius * sqrt(1.0f - (normal.x * normal.x));
    float radY = radius * sqrt(1.0f - (normal.y * normal.y));
    float radZ = radius * sqrt(1.0f - (normal.z * normal.z));
    Vector rad(radX, radY, radZ);
    return BBox(center - rad, center + rad);
}

Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const {
    float distance;
    Vector discOrigin(center.x, center.y, center.z);
    Vector rayOrigin(ray.o.x, ray.o.y, ray.o.z);

    distance = (dot(discOrigin, normal) - dot(rayOrigin, normal)) / dot(ray.d, normal);
    Point hitPoint = ray.o + distance * ray.d;
    Vector hitPointCenter = hitPoint - center;
    if ((distance >= 0) && (hitPointCenter.length() <= radius) && (distance < previousBestDistance)) {
        Intersection intersection(distance, ray, this, normal, hitPoint);
        return intersection;
    }
    else
        return Intersection::failure();
}

Solid::Sample Disc::sample() const {
    // find random point on disc.
    float rx, ry, rz;
    float u = random();
    float v = sqrt(random());
    float r1 = v * radius * cos(2 * pi * u);
    float r2 = v * radius * sin(2 * pi * u);
    float r3 = v * radius * sin(2 * pi * u);

    rx = r1 + center.x;
    ry = r2 + center.y;
    rz = r3 + center.z;

    Sample sample;
    sample.point = Point(rx, ry, rz);
    sample.normal = normal;
    return sample;
}

float Disc::getArea() const {
    return pi * radius * radius;
}

}
