#include "interpolate.h"

namespace rt {

template <typename T>
T lerp(const T& px0, const T& px1, float xPoint) {
    T p_1d = (1 - xPoint) * px0 + xPoint * px1;
    return p_1d;
}

template <typename T>
T lerpbar(const T& a, const T& b, const T& c, float aWeight, float bWeight) {
    float cWeight = 1 - aWeight - bWeight;
    T p = aWeight * a + bWeight * b + cWeight * c;
    return p;
}

template <typename T>
T lerp2d(const T& px0y0, const T& px1y0, const T& px0y1, const T& px1y1, float xWeight, float yWeight) {
    T pxy0 = lerp(px0y0, px1y0, xWeight);
    T pxy1 = lerp(px0y1, px1y1, xWeight);
    T p_2d = lerp(pxy0, pxy1, yWeight);
    return p_2d;
}

template <typename T>
T lerp3d(const T& px0y0z0, const T& px1y0z0, const T& px0y1z0, const T& px1y1z0,
    const T& px0y0z1, const T& px1y0z1, const T& px0y1z1, const T& px1y1z1,
    float xPoint, float yPoint, float zPoint)
{
    T pxyz0 = lerp2d(px0y0z0, px1y0z0, px0y1z0, px1y1z0, xPoint, yPoint);
    T pxyz1 = lerp2d(px0y0z1, px1y0z1, px0y1z1, px1y1z1, xPoint, yPoint);
    T p_3d = lerp(pxyz0, pxyz1, zPoint);
    return p_3d;
}

}