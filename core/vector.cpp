#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/float4.h>
#include <core/assert.h>
#include <algorithm>
#include <cmath>

namespace rt {

Vector::Vector(float x, float y, float z)
{
    // Using this pointer for initializing variables in constructor 
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector::Vector(const Float4& f4)
{
    assert(fabs(f4.w) <= epsilon);
    this->x = f4.x - f4.w;
    this->y = f4.y - f4.w;
    this->z = f4.z - f4.w;
}

Vector Vector::operator + (const Vector& b) const {
    Vector temp;
    temp.x = x + b.x;
    temp.y = y + b.y;
    temp.z = z + b.z;
    return temp;
}

Vector Vector::operator - (const Vector& b) const {
    Vector temp;
    temp.x = x - b.x;
    temp.y = y - b.y;
    temp.z = z - b.z;
    return temp;
}

Vector Vector::operator - () const {
    return -1 * Vector(x, y, z);
}

Vector Vector::normalize() const {
    Vector temp = Vector(x, y, z);
    float magnitude = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
    temp = temp / magnitude;

    return temp;
}

Vector operator * (float scalar, const Vector& b) {
    Vector temp;
    temp.x = b.x * scalar;
    temp.y = b.y * scalar;
    temp.z = b.z * scalar;

    return temp;
}

Vector operator * (const Vector& a, float scalar) {
    Vector temp;
    temp.x = a.x * scalar;
    temp.y = a.y * scalar;
    temp.z = a.z * scalar;

    return temp;
}

Vector operator / (const Vector& a, float scalar) {
    Vector temp;
    temp.x = a.x / scalar;
    temp.y = a.y / scalar;
    temp.z = a.z / scalar;

    return temp;
}

Vector cross(const Vector& a, const Vector& b) {
    Vector temp;
    temp.x = a.y*b.z - a.z*b.y;
    temp.y = a.z*b.x - a.x*b.z;
    temp.z = a.x*b.y - a.y*b.x;

    return temp;
}

float dot(const Vector& a, const Vector& b) {
    float dot_product = 0.0;

    dot_product = a.x * b.x + a.y * b.y + a.z * b.z;
    
    return dot_product;
}

float Vector::lensqr() const {
    return x * x + y * y + z * z;
}

float Vector::length() const {

    return sqrt(x * x + y * y + z * z);
}


bool Vector::operator == (const Vector& b) const {
    //return *this == b;

    bool isEqual;
    isEqual = (x == b.x && y ==b.y && z == b.z);
    return isEqual;
}

bool Vector::operator != (const Vector& b) const {
    //return !(*this ==b);
    bool isNotEqual;
    isNotEqual = !(x == b.x && y == b.y && z == b.z);
    return isNotEqual;
}

Vector min(const Vector& a, const Vector& b) {
    Vector temp;
    temp.x = min(a.x, b.x);
    temp.y = min(a.y, b.y);
    temp.z = min(a.z, b.z);

    return temp;
}

Vector max(const Vector& a, const Vector& b) {
    Vector temp;
    temp.x = max(a.x, b.x);
    temp.y = max(a.y, b.y);
    temp.z = max(a.z, b.z);

    return temp;
}

Point operator + (const Point& a, const Vector& b) {
    Point temp;
    temp.x = a.x + b.x;
    temp.y = a.y + b.y;
    temp.z = a.z + b.z;
    return temp;
}

Point operator + (const Vector& a, const Point& b) {
    Point temp;
    temp.x = a.x + b.x;
    temp.y = a.y + b.y;
    temp.z = a.z + b.z;
    return temp;
}

Point operator - (const Point& a, const Vector& b) {
    Point temp;
    temp.x = a.x - b.x;
    temp.y = a.y - b.y;
    temp.z = a.z - b.z;
    return temp;;
}

Point operator * (const Float4& scale, const Point& p) {
    /* TODO */ NOT_IMPLEMENTED;
}

}
