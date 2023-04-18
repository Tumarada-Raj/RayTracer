#include <core/point.h>
#include <core/float4.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>

namespace rt {

Point::Point(float x, float y, float z)
{
    Point::x = x;
    Point::y = y;
    Point::z = z;
}

Point::Point(const Float4& f4)
{
    assert(fabs(f4.w) > epsilon);
    this->x = f4.x / f4.w;
    this->y = f4.y / f4.w;
    this->z = f4.z / f4.w;
}

Vector Point::operator - (const Point& b) const {
    Vector temp;
    temp.x = x - b.x;
    temp.y = y - b.y;
    temp.z = z - b.z;
    return temp;
}

bool Point::operator == (const Point& b) const {
    bool isEqual;
    isEqual = (x - b.x < epsilon && y - b.y < epsilon && z - b.z < epsilon);
    return isEqual;
}

bool Point::operator != (const Point& b) const {
    bool isNotEqual;
    isNotEqual = !(x == b.x && y == b.y && z == b.z);
    return isNotEqual;
}

Point operator + (const Point& a, const Point& b) {   // Needed for triangle mapper
    return (Point(a.x + b.x, a.y + b.y, a.z + b.z));
}

Point operator * (float scalar, const Point& b) {
    Point temp;
    temp.x = b.x * scalar;
    temp.y = b.y * scalar;
    temp.z = b.z * scalar;

    return temp;
}

Point operator * (const Point& a, float scalar) {
    Point temp;
    temp.x = a.x * scalar;
    temp.y = a.y * scalar;
    temp.z = a.z * scalar;

    return temp;
}

Point min(const Point& a, const Point& b) {
    Point temp;
    temp.x = min(a.x, b.x);
    temp.y = min(a.y, b.y);
    temp.z = min(a.z, b.z);

    return temp;
}

Point max(const Point& a, const Point& b) {
    Point temp;
    temp.x = max(a.x, b.x);
    temp.y = max(a.y, b.y);
    temp.z = max(a.z, b.z);

    return temp;
}

float Point::getCoordinate(const int index) const {
    if (index == 0)
        return x;
    else if (index == 1)
        return y;
    else
        return z;
}

}
