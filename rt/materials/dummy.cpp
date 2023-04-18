#include <rt/materials/dummy.h>
#include <core/assert.h>
#include <math.h>

namespace rt {

DummyMaterial::DummyMaterial() {
    this->brdf = 1.0f;
}

RGBColor DummyMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    float reflectance = this->brdf * dot(normal, inDir);
    return RGBColor::rep(fabsf(reflectance));
}

RGBColor DummyMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance DummyMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    SampleReflectance temp;
    temp.direction = outDir; //arbitary direction vector.
    temp.reflectance = RGBColor::rep(0.f);
    return temp;
}

}