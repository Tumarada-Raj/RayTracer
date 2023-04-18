#include <rt/textures/perlin.h>
#include <cmath>

namespace rt {

// returns a value in range -1 to 1
static inline float noise(int x, int y, int z) {
    int n = x + y * 57 + z * 997;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

PerlinTexture::PerlinTexture(const RGBColor& white, const RGBColor& black)
{
    this->white = white;
    this->black = black;
}

rt::RGBColor PerlinTexture::getColor(const Point& coord) {
    Point p;
    float total = 0.0f;
    int lu, lv, lw;
    float fu, fv, fw, mu, mv, mw;
    float n1, n2, n3, n4, n5, n6, n7, n8;
    for (int i = 0; i < amplitudes.size(); i++) {
        p = frequencies[i] * coord;
        lu = floor(p.x); lv = floor(p.y); lw = floor(p.z);
        mu = ceil(p.x); mv = ceil(p.y); mw = ceil(p.z);
        fu = p.x - lu; fv = p.y - lv; fw = p.z - lw;
        n1 = fabs(noise(lu, lv, lw));
        n2 = fabs(noise(mu, lv, lw));
        n3 = fabs(noise(lu, mv, lw));
        n4 = fabs(noise(mu, mv, lw));
        n5 = fabs(noise(lu, lv, mw));
        n6 = fabs(noise(mu, lv, mw));
        n7 = fabs(noise(lu, mv, mw));
        n8 = fabs(noise(mu, mv, mw));
        total = total + (lerp3d(n1, n2, n3, n4, n5, n6, n7, n8, fu, fv, fw) * amplitudes[i]);
    }
    //total = (total + 1) / 2;
    return lerp(black, white, total);
}

rt::RGBColor PerlinTexture::getColorDX(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

rt::RGBColor PerlinTexture::getColorDY(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

void PerlinTexture::addOctave(float amplitude, float frequency) {
    amplitudes.push_back(amplitude);
    frequencies.push_back(frequency);
}

}