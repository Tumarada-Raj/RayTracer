#include <core/float4.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/assert.h>
#include <core/scalar.h>

namespace rt {

Float4::Float4(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Float4::Float4(const Point& p) {
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
    this->w = 1;
}

Float4::Float4(const Vector& vec) {
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = 0.f;
}

float& Float4::operator [] (int idx) {
    switch (idx) {
    case 0:
        return this->x;
    case 1:
        return this->y;
    case 2:
        return this->z;
    case 3:
        return this->w;
    }
}

float Float4::operator [] (int idx) const {
    float temp;
    switch (idx) {
    case 0:
        temp = this->x;
        break;
    case 1:
        temp = this->y;
        break;
    case 2:
        temp = this->z;
        break;
    case 3:
        temp = this->w;
        break;
    }
    return temp;
}

Float4 Float4::operator + (const Float4& b) const {
    Float4 temp = Float4();
    temp.x = this->x + b.x;
    temp.y = this->y + b.y;
    temp.z = this->z + b.z;
    temp.w = this->w + b.w;
    return temp;;
}

Float4 Float4::operator - (const Float4& b) const {
    Float4 temp = Float4();
    temp.x = this->x - b.x;
    temp.y = this->y - b.y;
    temp.z = this->z - b.z;
    temp.w = this->w - b.w;
    return temp;;
}

Float4 Float4::operator * (const Float4& b) const {
    Float4 temp = Float4();
    temp.x = this->x * b.x;
    temp.y = this->y * b.y;
    temp.z = this->z * b.z;
    temp.w = this->w * b.w;
    return temp;
}

Float4 Float4::operator / (const Float4& b) const {
    Float4 temp = Float4();
    temp.x = this->x / b.x;
    temp.y = this->y / b.y;
    temp.z = this->z / b.z;
    temp.w = this->w / b.w;
    return temp;
}

Float4 operator * (float scalar, const Float4& b) {
    Float4 temp = Float4();
    temp.x = scalar * b.x;
    temp.y = scalar * b.y;
    temp.z = scalar * b.z;
    temp.w = scalar * b.w;
    return temp;
}

Float4 operator * (const Float4& a, float scalar) {
    Float4 temp = Float4();
    temp.x = scalar * a.x;
    temp.y = scalar * a.y;
    temp.z = scalar * a.z;
    temp.w = scalar * a.w;
    return temp;
}

Float4 operator / (const Float4& a, float scalar) {
    Float4 temp = Float4();
    temp.x = a.x / scalar;
    temp.y = a.y / scalar;
    temp.z = a.z / scalar;
    temp.w = a.w / scalar;
    return temp;
}

float dot(const Float4& a, const Float4& b) {
    float temp = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    return temp;
}

Float4 Float4::operator - () const {
    return (*this) * (-1.0f);
}

bool Float4::operator == (const Float4& b) const {
    bool isEqual;

    isEqual = (x - b.x < epsilon&& y - b.y < epsilon&& z - b.z < epsilon && w - b.w < epsilon);
    return isEqual;
}

bool Float4::operator != (const Float4& b) const {
    bool isNotEqual;

    isNotEqual = !(x - b.x < epsilon&& y - b.y < epsilon&& z - b.z < epsilon && w - b.w < epsilon);
    return isNotEqual;
}

Float4 min(const Float4& a, const Float4& b) {
    Float4 temp = Float4();
    if (a.x < b.x)
        temp.x = a.x;
    else
        temp.x = b.x;
    if (a.y < b.y)
        temp.y = a.y;
    else
        temp.y = b.y;
    if (a.z < b.z)
        temp.z = a.z;
    else
        temp.z = b.z;
    if (a.w < b.w)
        temp.w = a.w;
    else
        temp.w = b.w;
    return temp;
}

Float4 max(const Float4& a, const Float4& b) {
    Float4 temp = Float4();
    if (a.x > b.x)
        temp.x = a.x;
    else
        temp.x = b.x;
    if (a.y > b.y)
        temp.y = a.y;
    else
        temp.y = b.y;
    if (a.z > b.z)
        temp.z = a.z;
    else
        temp.z = b.z;
    if (a.w > b.w)
        temp.w = a.w;
    else
        temp.w = b.w;
    return temp;
}

}