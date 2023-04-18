#ifndef CG1_RAYTRACER_MOVINGSPHERE_H
#define CG1_RAYTRACER_MOVINGSPHERE_H

#include <rt/solids/solid.h>

namespace rt {

    class MovingSphere : public Solid {
    public:
        MovingSphere(const Point& center0, const Point& center1, float radius, CoordMapper* texMapper, Material* material, float time0, float time1);

        virtual BBox getBounds() const;
        virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
        virtual Sample sample() const;
        virtual float getArea() const;

        Point move(float time) const;

    private:
        Point center0, center1;
        float radius;
        float time0, time1;
    };

}

#endif //CG1_RAYTRACER_MOVINGSPHERE_H