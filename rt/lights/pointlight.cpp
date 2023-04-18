#include <rt/lights/pointlight.h>

#include <core/vector.h>

namespace rt {

PointLight::PointLight(const Point& position, const RGBColor& intensity)
{
    this->position = position;
    this->intensity = intensity;
}

LightHit PointLight::getLightHit(const Point& p) const {
    LightHit temp;
    Vector hitPointToLight = position - p;
    temp.distance = hitPointToLight.length();
    temp.direction = hitPointToLight.normalize();
    temp.normal = -hitPointToLight;
    return temp;
}

RGBColor PointLight::getIntensity(const LightHit& irr) const {
    return intensity * (1.f / (irr.distance * irr.distance));
}

}
