#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>
#include <cmath>

namespace rt {

class Ray;
static const float minus_inf = -INFINITY;
static const float plus_inf = INFINITY;

class BBox {
public:
    Point min, max;
    bool isEmpty, isFull;

    BBox() {}
    BBox(const Point& min, const Point& max)
    {
        this->min = min;
        this->max = max;
        this->isEmpty = false;
        this->isFull = false;
    }

    BBox(bool isEmpty, bool isFull)
    {
        this->isEmpty = isEmpty;
        this->isFull = isFull;
        if (isFull) {
            min.x = minus_inf;
            min.y = minus_inf;
            min.z = minus_inf;
            max.x = plus_inf;
            max.y = plus_inf;
            max.z = plus_inf;
        }
    }

    static BBox empty();
    static BBox full();

    void extend(const Point& point);
    void extend(const BBox& bbox);

    Vector diagonal() const {
        return (max - min);
    }

    float area() const {
        float xlen = abs(max.x - min.x);
        float ylen = abs(max.y - min.y);
        float zlen = abs(max.z - min.z);
        return 2 * (xlen * ylen + ylen * zlen + zlen * xlen);
    }

    std::pair<float, float> intersect(const Ray& ray) const;

    bool isUnbound() const;
    Point getBBoxCentroid() const;
};

}

#endif