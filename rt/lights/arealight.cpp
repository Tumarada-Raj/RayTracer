#include <rt/lights/arealight.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <core/color.h>

namespace rt {

LightHit AreaLight::getLightHit(const Point& p) const {
    LightHit lh;
    float offset = 0.00015f;  //Set this value based on the reference images
    Solid::Sample sample = source->sample();

    lh.direction = (sample.point - p).normalize();
    lh.normal = sample.normal;
    lh.distance = (sample.point - p).length() - offset;

    return lh;
}

RGBColor AreaLight::getIntensity(const LightHit& irr) const {
    RGBColor emission = source->material->getEmission(Point::rep(0.f), Vector::rep(0.f), Vector::rep(0.f));
    RGBColor power = emission * source->getArea();
    return power * (dot(irr.normal, -irr.direction) / (irr.distance * irr.distance));  //cosine/dist^2
}

AreaLight::AreaLight(Solid* source)
{
    this->source = source;
}

}