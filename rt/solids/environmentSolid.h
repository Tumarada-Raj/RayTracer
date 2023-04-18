#ifndef CG1_RAYTRACER_ENVIROMENTMAP_H
#define CG1_RAYTRACER_ENVIROMENTMAP_H

#include <rt/solids/solid.h>

namespace rt {

    class EnvironmentSolid : public Solid {
    public:
        EnvironmentSolid() {}
        EnvironmentSolid(CoordMapper* texMapper, Material* material);

        virtual BBox getBounds() const;
        virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
        virtual Sample sample() const;
        virtual float getArea() const;

    private:
        Material* material;
        CoordMapper* texMapper;
    };

}

#endif //CG1_RAYTRACER_ENVIROMENTMAP_H