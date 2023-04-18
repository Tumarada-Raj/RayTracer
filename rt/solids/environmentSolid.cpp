#include <rt/solids/environmentSolid.h>
#include<cmath>

namespace rt {

    EnvironmentSolid::EnvironmentSolid(CoordMapper* texMapper, Material* material) : Solid(texMapper, material) {
    }

    Intersection EnvironmentSolid::intersect(const rt::Ray& ray, float previousBestDistance) const {
        // always intersect at infinity.
        //return Intersection(FLT_MAX, ray, this, -ray.d, Point(0, 0, 0));
        if (previousBestDistance == FLT_MAX) {
            //Ray newr(Point(FLT_MAX, FLT_MAX, FLT_MAX), Vector(-1, -1, -1));
            return Intersection(FLT_MAX/10, ray, this, -ray.d, Point(0, 0, 0));

        }
        else
            return Intersection::failure();

    }

    BBox EnvironmentSolid::getBounds() const {
        return BBox::full();
    }

    float EnvironmentSolid::getArea() const {
        return FLT_MAX;
    }

    Solid::Sample EnvironmentSolid::sample() const {
        NOT_IMPLEMENTED;
    }

};