#include <rt/cameras/perspective.h>
#include <cmath>
#include <core/random.h>

namespace rt {

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, float time0, float time1)
{
    this->center = center;
    this->forward = forward;
    this->up = up;
    this->verticalOpeningAngle = verticalOpeningAngle;
    this->horizontalOpeningAngle = horizontalOpeningAngle;
    this->w = -forward.normalize();
    this->u = cross(up, w).normalize();
    this->v = cross(w, u).normalize();
    this->time0 = time0;
    this->time1 = time1;
}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
    Vector d;

    //Took reference from Peter Shirley textbook pages 73-76 for setting camera coordinate system.
    d = (-w + u * x * tan(horizontalOpeningAngle / 2) + v * y * tan(verticalOpeningAngle / 2)).normalize();
    float time = time0 + random() * (time1 - time0);

    return Ray(center, d, time);
}

}
