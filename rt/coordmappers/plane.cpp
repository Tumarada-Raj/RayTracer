#include <rt/coordmappers/plane.h>

namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2) {
    this->e1 = e1;
    this->e2 = e2;
    this->origin = Point::rep(0.0f);
    this->normal = cross(this->e1, this->e2);
    this->m = Matrix::system(this->e1, this->e2, normal).invert();
}

Point PlaneCoordMapper::getCoords(const Intersection& hit) const {
    Point temp = m * hit.local();
    return temp;
}

}