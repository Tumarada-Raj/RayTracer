#include <rt/primmod/instance.h>

namespace rt {

Instance::Instance(Primitive* content)
{
    archetype = content;
    transf = Matrix::identity();
    transfInv = transf;
    computeBounds();
}

Primitive* Instance::content() {
    return archetype;
}

void Instance::reset() {
    transf = Matrix::identity();
    transfInv = transf;
    computeBounds();
}

void Instance::translate(const Vector& t) {
    Matrix tMatrix{
            Float4(1.f, 0.f, 0.f, t.x),
            Float4(0.f, 1.f, 0.f, t.y),
            Float4(0.f, 0.f, 1.f, t.z),
            Float4(0.f, 0.f, 0.f, 1.f)
    };
    transf = product(tMatrix, transf);
    transfInv = transf.invert();
    computeBounds();
}

void Instance::rotate(const Vector& nnaxis, float angle) {
    Vector r = nnaxis.normalize();
    float x = r.x, y = r.y, z = r.z;
    Vector s;

    float minVal = min(abs(x), abs(y), abs(z));
    if (x == minVal)
        s = Vector(0.f, -z, y);
    else if (y == minVal)
        s = Vector(-z, 0.f, x);
    else if (z == minVal)
        s = Vector(-y, x, 0.f);

    s = s.normalize();

    assert(s != Vector::rep(0.f));
    assert(dot(s, r) == 0); // must be orthogonal.

    Vector t = cross(r, s);

    Matrix m = Matrix::system(r, s, t);
    Matrix Rx{
        Float4(1.f, 0.f    , 0.f      ,0.f),
        Float4(0.f, cos(angle) , -sin(angle)  ,0.f),
        Float4(0.f, sin(angle), cos(angle)  ,0.f),
        Float4(0.f, 0.f    , 0.f     ,1.f)
    };
    transf = product(product(m, Rx), m.transpose());
    transfInv = transf.invert();
    computeBounds();
}

void Instance::scale(float f) {
    Matrix tMatrix{
            Float4(f, 0.f, 0.f, 0.f),
            Float4(0.f, f, 0.f, 0.f),
            Float4(0.f, 0.f, f, 0.f),
            Float4(0.f, 0.f, 0.f, 1.f)
    };
    transf = product(tMatrix, transf);
    transfInv = transf.invert();
    computeBounds();
}

void Instance::scale(const Vector& s) {
    Matrix tMatrix{
            Float4(s.x, 0.f, 0.f, 0.f),
            Float4(0.f, s.y, 0.f, 0.f),
            Float4(0.f, 0.f, s.z, 0.f),
            Float4(0.f, 0.f, 0.f, 1.f)
    };
    transf = product(tMatrix, transf);
    transfInv = transf.invert();
    computeBounds();
}

void Instance::setMaterial(Material* m) {
    archetype->setMaterial(m);
}

void Instance::setCoordMapper(CoordMapper* cm) {
    archetype->setCoordMapper(cm);
}

Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const {
    Point invRayOrig = transfInv * ray.o;
    Vector invRayDir = transfInv * ray.d;

    float dirLength = invRayDir.length();
    Ray rayTrans(invRayOrig, invRayDir.normalize());

    Intersection intersectionTrans = this->archetype->intersect(rayTrans, previousBestDistance * dirLength);

    if (intersectionTrans) {
        Vector normal = transfInv.transpose() * intersectionTrans.normal();
        Intersection intersection(intersectionTrans.distance / dirLength, ray, intersectionTrans.solid, normal.normalize(), intersectionTrans.local());
        return intersection;
    }
    else
        return intersectionTrans;
}

void Instance::computeBounds() {
    BBox archBounds = archetype->getBounds();
    /*Point Pmin = min(archBounds.min, archBounds.max);
    Point Pmax = max(archBounds.min, archBounds.max);
    instanceBBox = BBox(transf * Pmin, transf * Pmax);*/
   Point c1(archBounds.min.x, archBounds.min.y, archBounds.min.z);
    Point c2(archBounds.max.x, archBounds.min.y, archBounds.min.z);
    Point c3(archBounds.min.x, archBounds.max.y, archBounds.min.z);
    Point c4(archBounds.min.x, archBounds.min.y, archBounds.max.z);
    Point c5(archBounds.min.x, archBounds.max.y, archBounds.max.z);
    Point c6(archBounds.max.x, archBounds.max.y, archBounds.min.z);
    Point c7(archBounds.max.x, archBounds.min.y, archBounds.max.z);
    Point c8(archBounds.max.x, archBounds.max.y, archBounds.max.z);
    Point c1Trans = transf * c1;
    Point c2Trans = transf * c2;
    Point c3Trans = transf * c3;
    Point c4Trans = transf * c4;
    Point c5Trans = transf * c5;
    Point c6Trans = transf * c6;
    Point c7Trans = transf * c7;
    Point c8Trans = transf * c8;
    float minx = min(
        min(c1Trans.x, c2Trans.x, c3Trans.x),
        min(c4Trans.x, c5Trans.x, c6Trans.x),
        min(c7Trans.x, c8Trans.x));
    float miny = min(
        min(c1Trans.y, c2Trans.y, c3Trans.y),
        min(c4Trans.y, c5Trans.y, c6Trans.y),
        min(c7Trans.y, c8Trans.y));
    float minz = min(
        min(c1Trans.z, c2Trans.z, c3Trans.z),
        min(c4Trans.z, c5Trans.z, c6Trans.z),
        min(c7Trans.z, c8Trans.z));
    float maxx = max(
        max(c1Trans.x, c2Trans.x, c3Trans.x),
        max(c4Trans.x, c5Trans.x, c6Trans.x),
        max(c7Trans.x, c8Trans.x));
    float maxy = max(
        max(c1Trans.y, c2Trans.y, c3Trans.y),
        max(c4Trans.y, c5Trans.y, c6Trans.y),
        max(c7Trans.y, c8Trans.y));
    float maxz = max(
        max(c1Trans.z, c2Trans.z, c3Trans.z),
        max(c4Trans.z, c5Trans.z, c6Trans.z),
        max(c7Trans.z, c8Trans.z));

    Point min(minx, miny, minz);
    Point max(maxx, maxy, maxz);
    instanceBBox = BBox(min, max);

}

BBox Instance::getBounds() const {
    return instanceBBox;
}

}