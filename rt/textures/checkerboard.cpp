#include <rt/textures/checkerboard.h>
#include <cmath>

namespace rt {

CheckerboardTexture::CheckerboardTexture(const RGBColor& white, const RGBColor& black)
{
    this->white = white;
    this->black = black;
}

RGBColor CheckerboardTexture::getColor(const Point& coord) {

    int lu = floor(coord.x * 2.0f);
    int lv = floor(coord.y * 2.0f);
    int lw = floor(coord.z * 2.0f);
    int parity = (lu + lv + lw) % 2;
    if (parity == 0)
        return white;
    else
        return black;
}

RGBColor CheckerboardTexture::getColorDX(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor CheckerboardTexture::getColorDY(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

}