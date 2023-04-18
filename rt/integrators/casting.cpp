#include <rt/integrators/casting.h>
#include <rt/world.h>

namespace rt {

RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const {
    /* TODONOT_IMPLEMENTED;*/
    Intersection getColor = world->scene->intersect(ray);
    if (getColor) {
        float value = -1.0 * dot(ray.d.normalize(), getColor.normal());
        RGBColor color = RGBColor::rep(value).clamp();
        return color;
    }
    return RGBColor(0, 0, 0);
}

}
