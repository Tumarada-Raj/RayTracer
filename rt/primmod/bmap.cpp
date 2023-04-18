#include <rt/primmod/bmap.h>
#include <core/assert.h>
#include <rt/solids/triangle.h>
#include <rt/textures/texture.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/coordmappers/tmapper.h>

namespace rt {

BumpMapper::BumpMapper(Triangle* base, Texture* bumpmap, const Point& bv1, const Point& bv2, const Point& bv3, float vscale)
{
    this->base = base;
    this->bumpmap = bumpmap;
    this->vscale = vscale;

    cmapper = new TriangleMapper(bv1, bv2, bv3);
    O_u = Vector(1, 0, 0);
    O_v = Vector(0, 1, 0);
}

BBox BumpMapper::getBounds() const {
    return base->getBounds();
}

// TODO: A full implementation has to make use of the coord mappers as well!
Intersection BumpMapper::intersect(const Ray& ray, float previousBestDistance) const {
    Intersection inter = base->intersect(ray, previousBestDistance);

    if (inter) {
        Point p = cmapper->getCoords(inter);

        auto dX = bumpmap->getColorDX(p);
        auto dY = bumpmap->getColorDY(p);

        Vector N = inter.normal();
        Vector D = dX.g * cross(O_v, N) + dY.g * cross(N, O_u);

        N = (N - D).normalize();
        inter.setNormal(N);

        return inter;
    }

    return Intersection::failure();
}

void BumpMapper::setMaterial(Material* m) {
    this->base->setMaterial(m);
}

void BumpMapper::setCoordMapper(CoordMapper* cm) {
    this->base->setCoordMapper(cm);
}

}