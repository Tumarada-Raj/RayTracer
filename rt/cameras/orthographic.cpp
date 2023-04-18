#include <rt/cameras/orthographic.h>
#include <rt/ray.h>

namespace rt {

OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY)
{
    this->center = center;
    this->forward = forward;
    this->up = up;
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    this->w = -forward.normalize();
    this->u = cross(up, w).normalize();
    this->v = cross(w, u).normalize();
}

Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
    Ray temp;

    //Took reference from Peter Shirley textbook pages 73-76 for setting camera coordinate system.
    temp.o = center + u * (x/2 * scaleX) + v * (y/2 * scaleY);
    temp.d = -w;

    return temp;
}

}
