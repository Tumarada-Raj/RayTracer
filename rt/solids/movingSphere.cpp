#include <rt/solids/movingSphere.h>

namespace rt {

    MovingSphere::MovingSphere(const Point& center0, const Point& center1, float radius, CoordMapper* texMapper, Material* material, float time0, float time1)
    {
        /* TODO */
        this->center0 = center0;
        this->center1 = center1;
        this->radius = radius;
        this->material = material;
        if (texMapper != nullptr) this->texMapper = texMapper;

        this->time0 = time0;
        this->time1 = time1;
    }


    BBox MovingSphere::getBounds() const {
        /* TODO */
        Point min(center0.x - radius, center0.y - radius, center0.z - radius);
        Point max(center0.x + radius, center0.y + radius, center0.z + radius);
        return BBox(min, max);
    }

    Intersection MovingSphere::intersect(const Ray& ray, float previousBestDistance) const {
        /* TODO */
        float distance;
        Vector oc = ray.o - move(ray.time);
        float a = dot(ray.d, ray.d);
        float b = 2.0f * dot(oc, ray.d);
        float c = dot(oc, oc) - (radius * radius);

        float discriminant = (b * b) - (4 * a * c);
        if (discriminant >= 0.0f) {
            float d0 = (-b + sqrt(discriminant)) / (2 * a);
            float d1 = (-b - sqrt(discriminant)) / (2 * a);
            if (d1 < d0)
                std::swap(d0, d1);
            if (d0 < 0) {
                d0 = d1; // if d0 is negative, let's use d1 instead
                if (d0 < 0)
                    return Intersection::failure(); // both d0 and d1 are negative
            }
            distance = d0;
        }
        else
            return Intersection::failure();

        if (distance < previousBestDistance) {
            Vector normal = ray.getPoint(distance) - move(ray.time);
            Intersection intersection(distance, ray, this, normal.normalize(), ray.getPoint(distance));
            return intersection;
        }
        else
            return Intersection::failure();
    }

    Solid::Sample MovingSphere::sample() const {
        NOT_IMPLEMENTED;
    }

    float MovingSphere::getArea() const {
        /* TODO */
        return 4 * pi * radius * radius;
    }

    Point MovingSphere::move(const float time) const {
        return center0 + (time - time0) / (time1 - time0) * (center1 - center0);
    }

}