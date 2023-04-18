#include <rt/groups/simplegroup.h>

namespace rt {

BBox SimpleGroup::getBounds() const {
    BBox groupBbox = BBox::empty();
    BBox primitiveBbox;
    for (auto iter = primitives.begin(); iter != primitives.end(); ++iter) {
        primitiveBbox = (*iter)->getBounds();
        groupBbox.extend(primitiveBbox);
    }
    return groupBbox;
}

Intersection SimpleGroup::intersect( const Ray& ray, float previousBestDistance) const {
    Intersection nearestRayIntersection = Intersection::failure();
    Intersection currRayIntersection;
    for (auto i = primitives.begin(); i != primitives.end(); i++)
    {
        currRayIntersection = (*i)->intersect(ray, previousBestDistance);
        if (currRayIntersection) 
        { 
            nearestRayIntersection = currRayIntersection;
            previousBestDistance = nearestRayIntersection.distance;
        }
    }
    return nearestRayIntersection;
}

void SimpleGroup::rebuildIndex() {
    //do nothing
}

void SimpleGroup::add(Primitive* p) {
    primitives.push_back(p);
}

void SimpleGroup::setMaterial(Material* m) {
    for (auto iter = primitives.begin(); iter != primitives.end(); ++iter) {
        (*iter)->setMaterial(m);
    }
}

void SimpleGroup::setCoordMapper(CoordMapper* cm) {
    for (auto iter = primitives.begin(); iter != primitives.end(); ++iter) {
        (*iter)->setCoordMapper(cm);
    }
}

}
