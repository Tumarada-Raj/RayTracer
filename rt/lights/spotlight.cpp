#include <rt/lights/spotlight.h>

namespace rt {

SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity)
{
    this->position = position;
    this->direction = direction.normalize();
    this->angle = angle;
    this->exp = power;
    this->intensity = intensity;
}

LightHit SpotLight::getLightHit(const Point& p) const {

    LightHit temp;
    Vector hitPointToLight = position - p;
    temp.distance = hitPointToLight.length();
    temp.direction = hitPointToLight.normalize();
    temp.normal = -hitPointToLight;
    return temp;
}

RGBColor SpotLight::getIntensity(const LightHit& irr) const {
    float cos_irr = dot(-irr.direction, this->direction);
    if (angle > acos(cos_irr))
        return intensity * (pow(cos_irr, exp) / (irr.distance * irr.distance));
    else
        return RGBColor::rep(0.0f);
}

}
