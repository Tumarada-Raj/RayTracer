#include <rt/integrators/castingdist.h>
#include <rt/world.h>

namespace rt {

RayCastingDistIntegrator::RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist)
    : Integrator(world)
{
    this->nearColor = nearColor;
    this->nearDist = nearDist;
    this->farColor = farColor;
    this->farDist = farDist;
}

RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersection = world->scene->intersect(ray);
    RGBColor scalingFactor = (farColor - nearColor) / (farDist - nearDist);

    if (intersection) {

        RGBColor interpolColor;
        if (intersection.distance < nearDist)
            interpolColor = nearColor;
        else if (intersection.distance > farDist)
            interpolColor = farColor;
        else
            interpolColor = scalingFactor * (intersection.distance - nearDist) + nearColor;

        //- to get +ve color or else getting black scene :  Reason is Cosine is negative if intersected from the front ->flip sign.
        float value = dot(-ray.d.normalize(), intersection.normal().normalize());
        RGBColor grayValue = RGBColor::rep(value).clamp();

        return interpolColor * grayValue;
    }
    return RGBColor(0, 0, 0);
}

}
