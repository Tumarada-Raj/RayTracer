#include <rt/solids/triangle.h>
#include <core/random.h>

namespace rt {

    Triangle::Triangle(Point vertices[3], CoordMapper* texMapper, Material* material)
    {
        /* TODO */
        this->v1 = vertices[0];
        this->v2 = vertices[1];
        this->v3 = vertices[2];
        this->material = material;
        if (texMapper != nullptr) this->texMapper = texMapper;
        this->edge1 = v2 - v1;
        this->edge2 = v3 - v1;
        this->normal = cross(edge1, edge2).normalize();
        this->area = getArea();
    }

Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material)
:Solid(texMapper, material){
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->edge1 = v2 - v1;
    this->edge2 = v3 - v1;
    this->normal = cross(edge1, edge2).normalize();
    this->area = getArea();

}

BBox Triangle::getBounds() const {
    Point min, max;
    min.x = rt::min(v1.x, v2.x, v3.x);
    min.y = rt::min(v1.y, v2.y, v3.y);
    min.z = rt::min(v1.z, v2.z, v3.z);

    max.x = rt::max(v1.x, v2.x, v3.x);
    max.y = rt::max(v1.y, v2.y, v3.y);
    max.z = rt::max(v1.z, v2.z, v3.z);

    return BBox(min, max);
}

Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const {
    const float EPSILON = 0.0000001;
    Vector h, s, q;
    Vector rayVector = ray.d;
    Point rayOrigin = ray.o;
    float a, f, u, v;

    // Calculate determinent for êMoller algo. Also used in Barycentric coordinate u
    h = cross(rayVector, edge2);            // First part of scalar triple product
    a = dot(edge1, h);                     //determinent
    if (a == 0.f)
        return Intersection::failure();   // If det is 0 divide by 0 error/no unique solution ie., ray is parallel
    
    f = 1.0 / a;
    s = rayOrigin - v1;
    u = f * dot(s, h);      //Barycentric u coordinate
    if (u < 0.0 || u > 1.0) //If barycentric coordinate exceeds bounds i.e., ray missed triangle 
        return Intersection::failure();
    
    q = cross(s, edge1);  //Barycentric v coordinate
    v = f * dot(rayVector, q);
    if (v < 0.0 || u + v > 1.0) //If barycentric coordinate exceeds bounds i.e., ray missed triangle i.e., sum less than 1
        return Intersection::failure();
    
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * dot(edge2, q);
    if ((t > EPSILON) && (t < previousBestDistance)) // ray intersection
    {
        return Intersection(t, ray, this, normal, getBaryCoords(ray.getPoint(t)));
    }
    else // This means that there is a line intersection but not a ray intersection.
        return  Intersection::failure();
}

Point Triangle::getBaryCoords(const Point& p) const {
    Point bary;
    bary.x = getArea(v2 - p, v3 - p) / area;
    bary.y = getArea(v1 - p, v3 - p) / area;
    bary.z = getArea(v1 - p, v2 - p) / area;
    assert(bary.x >= 0 && bary.y >= 0 && bary.z >= 0);
    return bary;
}

Solid::Sample Triangle::sample() const {
    float r = random();
    float t = random();

    if ((r + t) > 1.f) {
        r = 1.f - r;
        t = 1.f - t;
    }

    Point p = (1 - r - t) * v1 + r * v2 + t * v3;;

    Sample sample;
    sample.point = p;
    sample.normal = normal;
    return sample;
}

float Triangle::getArea(const Vector& edge1, const Vector& edge2) const {
    return cross(edge1, edge2).length() / 2;
}

float Triangle::getArea() const {
    return getArea(edge1, edge2);
}

}