#include <rt/intersection.h>

namespace rt {

Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local)
{
    this->distance = distance;
    this->ray = ray;
    this->solid = solid;
    this->normalVec = normal;
    this->localP = local;
    this->isIntersect = true;
}

Intersection::operator bool() const {
    /* TODO  NOT_IMPLEMENTED;**/
    return this->isIntersect || this->solid != nullptr;
}

Intersection Intersection::failure() {
    /* TODO  NOT_IMPLEMENTED;*/
    Intersection fail = Intersection();
    fail.isIntersect = false;
    fail.solid = nullptr;
    return fail; //returns no intersection i.e., intersection failure case
}

Point Intersection::hitPoint() const {
    /* TODO  NOT_IMPLEMENTED;*/
    Point temp = ray.o + distance * ray.d;

    return temp;
}

Vector Intersection::normal() const {
    return normalVec;
}

Point Intersection::local() const {
    return localP;
}

Vector Intersection::setNormal(const Vector n) {
    normalVec = n;
    return normalVec;
}

}
