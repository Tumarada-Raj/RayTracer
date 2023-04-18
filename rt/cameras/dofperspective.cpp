#include <rt/cameras/dofperspective.h>
#include <rt/ray.h>
#include <cmath>
#include <core/random.h>

namespace rt {

DOFPerspectiveCamera::DOFPerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, float focalDistance, float apertureRadius)
{
    this->center = center;
    this->forward = forward;
    this->up = up;
    this->verticalOpeningAngle = verticalOpeningAngle;
    this->horizontalOpeningAngle = horizontalOpeningAngle;
    this->apertureRadius = apertureRadius;
    this->focalDistance = focalDistance;
    this->w = -forward.normalize();
    this->u = cross(up, w).normalize();
    this->v = cross(w, u).normalize();
}

Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const {
    Vector d;

    //Took reference from Peter Shirley textbook pages 73-76 for setting camera coordinate system.
    //temp.o = center;
    d = (-w + u * x * tan(horizontalOpeningAngle / 2) + v * y * tan(verticalOpeningAngle / 2)).normalize();

    Point focalPoint = center + focalDistance * d;
    float a = random(); float b = random(); float c = random(); float e = random();
    if (a < 0.5)
        b = -b;
    if (c < 0.5)
        e = -e;
    Point rayOrigin = center + b * apertureRadius * u + e * apertureRadius * v;
    Vector rayDirection = (focalPoint - rayOrigin).normalize();

    return Ray(rayOrigin, rayDirection);
}

}