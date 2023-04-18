#include <rt/bbox.h>
#include <rt/solids/aabox.h>


namespace rt {

BBox BBox::empty() {
    bool isEmpty = true;
    bool isFull = false;
    return BBox(isEmpty, isFull);
}

BBox BBox::full() {
    bool isEmpty = false;
    bool isFull = true;
    return BBox(isEmpty, isFull);
}


void BBox::extend(const Point& point) {
    if (this->isEmpty) {                    //Initial case for BBox if it's empty
        min = point;
        max = point;
        this->isEmpty = false;
    }
    else {                                  // Case if BBox is not empty
        min.x = std::min(point.x, min.x);
        min.y = std::min(point.y, min.y);
        min.z = std::min(point.z, min.z);

        max.x = std::max(point.x, max.x);
        max.y = std::max(point.y, max.y);
        max.z = std::max(point.z, max.z);
    }
}

void BBox::extend(const BBox& bbox) {         
    if (this->isEmpty) {                    //same as above but this time it's box object instead of a point
        min = bbox.min;
        max = bbox.max;
        this->isEmpty = false;
    }
    else {
        min.x = std::min(bbox.min.x, min.x);
        min.y = std::min(bbox.min.y, min.y);
        min.z = std::min(bbox.min.z, min.z);

        max.x = std::max(bbox.max.x, max.x);
        max.y = std::max(bbox.max.y, max.y);
        max.z = std::max(bbox.max.z, max.z);
    }
}

std::pair<float, float> BBox::intersect(const Ray& ray) const {
    if (isFull)
        return std::make_pair(-FLT_MAX, FLT_MAX);
    else if (isEmpty)
        return std::make_pair(FLT_MAX, -FLT_MAX);
    else {
        //AABox aabox(min, max, nullptr, nullptr);
        //Compute intersection
        float xnear, xfar, ynear, yfar, znear, zfar;
        float invdx = 1.0f / ray.d.x;
        float invdy = 1.0f / ray.d.y;
        float invdz = 1.0f / ray.d.z;
        // if the ray goes in negative direction, far and near are changed
        if (invdx >= 0) {
            xnear = (min.x - ray.o.x) * invdx;
            xfar = (max.x - ray.o.x) * invdx;
        }
        else {
            xfar = (min.x - ray.o.x) * invdx;
            xnear = (max.x - ray.o.x) * invdx;
        }
        if (invdy >= 0) {
            ynear = (min.y - ray.o.y) * invdy;
            yfar = (max.y - ray.o.y) * invdy;
        }
        else {
            yfar = (min.y - ray.o.y) * invdy;
            ynear = (max.y - ray.o.y) * invdy;
        }
        if (invdz >= 0) {
            znear = (min.z - ray.o.z) * invdz;
            zfar = (max.z - ray.o.z) * invdz;
        }
        else {
            zfar = (min.z - ray.o.z) * invdz;
            znear = (max.z - ray.o.z) * invdz;
        }

        float maxNear;
        float minfar;

        // Try intersection on xy plane
        if ((xnear > yfar) || (ynear > xfar))
            return std::make_pair(FLT_MAX, -FLT_MAX); //miss
        maxNear = std::max(xnear, ynear);
        minfar = std::min(xfar, yfar);

        // Try intersection on prev and z plane
        if ((maxNear > zfar) || (znear > minfar))
            return std::make_pair(FLT_MAX, -FLT_MAX); //miss
        maxNear = std::max(maxNear, znear);
        minfar = std::min(minfar, zfar);

        // Hit
        return std::make_pair(maxNear, minfar);
    }
}

bool BBox::isUnbound() const {
    if ((min.x == minus_inf && max.x == plus_inf)
        || (min.y == minus_inf && max.y == plus_inf)
        || (min.z == minus_inf && max.z == plus_inf))
        return true;
    else return false;
}


Point BBox::getBBoxCentroid() const{ 
    Point centroid; 
    centroid.x = (min.x + max.x)/2;
    centroid.y = (min.y + max.y)/2;
    centroid.z = (min.z + max.z)/2;
    return centroid;
}
}