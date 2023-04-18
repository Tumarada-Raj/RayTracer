#ifndef CG1RAYTRACER_INTEGRATORS_RAYTRACINGBLUR_HEADER
#define CG1RAYTRACER_INTEGRATORS_RAYTRACINGBLUR_HEADER

#include <rt/integrators/integrator.h>
#include <core/vector.h>

namespace rt {
    class World;
    class Ray;
    class RGBColor;
    class Vector;

    class RayTracingBlurIntegrator : public Integrator {
    public:
        RayTracingBlurIntegrator(World* world) : Integrator(world) { }
        virtual RGBColor getRadiance(const Ray& ray) const;
    private:
        float offset = 0.0001f;
        Vector scale0 = Vector::rep(1.f);
        Vector scale1 = Vector::rep(1.1f);
        Vector translate0 = Vector::rep(0.f);
        Vector translate1 = Vector(0.01f, 0.f, 0.f);

    };

}

#endif 