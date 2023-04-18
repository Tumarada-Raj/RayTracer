#include <rt/lights/directional.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& color)
{
    this->direction = direction;
    this->intensity = color;
}

LightHit DirectionalLight::getLightHit(const Point& p) const {
    LightHit temp;
    temp.direction = -direction;
    temp.distance = FLT_MAX;
    temp.normal = direction;
    return temp;
}

RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
    return intensity;
}

}
