#include <core/color.h>
#include <core/scalar.h>
#include <core/float4.h>

namespace rt {

RGBColor::RGBColor(const Float4& f4)
{
/* TODO */ NOT_IMPLEMENTED;
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
    RGBColor temp;

    temp.r = r + c.r;
    temp.g = g + c.g;
    temp.b = b + c.b;

    return temp;
}

RGBColor RGBColor::operator - (const RGBColor& c) const {
    RGBColor temp;

    temp.r = r - c.r;
    temp.g = g - c.g;
    temp.b = b - c.b;

    return temp;
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
    RGBColor temp;

    temp.r = r * c.r;
    temp.g = g * c.g;
    temp.b = b * c.b;

    return temp;
}

bool RGBColor::operator == (const RGBColor& c) const {
    bool isEqual;

    isEqual = (r - c.r < epsilon && g - c.g < epsilon && b - c.b < epsilon);
    return isEqual;
}

bool RGBColor::operator != (const RGBColor& b) const {
    bool isNotEqual;

    isNotEqual = !(r - b.r < epsilon && g - b.g < epsilon && RGBColor::b - b.b < epsilon);
    return isNotEqual;
}

RGBColor RGBColor::clamp() const {
    RGBColor temp;

    if (r < 0) { temp.r = 0; }
    else if (r > 1) { temp.r = 1; }
    else { temp.r = r; }

    if (g < 0) { temp.g = 0; }
    else if (g > 1) { temp.g = 1; }
    else { temp.g = g; }

    if (b < 0) { temp.b = 0; }
    else if (b > 1) { temp.b = 1; }
    else { temp.b = b; }
    
    return temp;
}

RGBColor RGBColor::gamma(float gam) const {
    /* TODO */ NOT_IMPLEMENTED;
}

float RGBColor::luminance() const {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor operator * (float scalar, const RGBColor& c) {
    RGBColor temp;

    temp.r = c.r * scalar;
    temp.g = c.g * scalar;
    temp.b = c.b * scalar;

    return temp;
}

RGBColor operator * (const RGBColor& c, float scalar) {
    RGBColor temp;

    temp.r = c.r * scalar;
    temp.g = c.g * scalar;
    temp.b = c.b * scalar;

    return temp;
}

RGBColor operator / (const RGBColor& c, float scalar) {
    RGBColor temp;

    temp.r = c.r / scalar;
    temp.g = c.g / scalar;
    temp.b = c.b / scalar;

    return temp;
}

}
