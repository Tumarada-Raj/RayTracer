#include <rt/solids/quad.h>
#include <core/random.h>

namespace rt {

    Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material)
    {
        this->span1 = span1;
        this->span2 = span2;
        this->v1 = origin;
        this->v2 = origin + span1;
        this->v3 = origin + span2;
        this->v4 = origin + span1 + span2;
        this->t1 = new Triangle(v1, v2, v3, nullptr, nullptr);
        this->t2 = new Triangle(v3, v2, v4, nullptr, nullptr);
        this->material = material;
        if (texMapper != nullptr) this->texMapper = texMapper;
    }

    BBox Quad::getBounds() const {
        BBox b1 = t1->getBounds();
        BBox b2 = t2->getBounds();
        b1.extend(b2);
        return b1;
    }

    Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {
        Intersection intersectionTri, intersection = Intersection::failure();
        Intersection i1 = t1->intersect(ray, previousBestDistance);
        if (i1)
            intersectionTri = i1;
        else {
            Intersection i2 = t2->intersect(ray, previousBestDistance);
            intersectionTri = i2;
        }
        if (intersectionTri) {
            intersection = Intersection(intersectionTri.distance, intersectionTri.ray, this, intersectionTri.normal(), getBaryCoords(intersectionTri.hitPoint()));
        }
        return intersection;
    }

    Point Quad::getBaryCoords(const Point& p) const{

    Vector op = p - v1;
    float u, v;
    if (span1.z * span2.y != 0.0f || span2.z * span1.y != 0.0f){    
        u = (op.z * span2.y - op.y * span2.z)/(span1.z * span2.y - span1.y * span2.z);
        v = (op.z * span1.y - op.y * span1.z)/(span2.z * span1.y - span2.y * span1.z);
    }
    else if(span1.x * span2.y != 0.0f || span1.y * span2.x != 0.0f){
        u = (op.x * span2.y - op.y * span2.x)/(span1.x * span2.y - span1.y * span2.x);
        v = (op.x * span1.y - op.y * span1.x)/(span2.x * span1.y - span2.y * span1.x);
    }    
    else if(span1.z * span2.x != 0.0f || span1.x * span2.z != 0.0f){
        u = (op.z * span2.x - op.x * span2.z)/(span1.z * span2.x - span1.x * span2.z);
        v = (op.z * span1.x - op.x * span1.z)/(span2.z * span1.x - span2.x * span1.z);

    }   
    Point bary(u, v, 0.0f);
    return bary;
}


    Solid::Sample Quad::sample() const {
        if (random() < .5f) //randomly sample from one triangle.
            return t1->sample();
        else
            return t2->sample();
    }

    float Quad::getArea() const {
        return t1->getArea() + t2->getArea();
    }

}