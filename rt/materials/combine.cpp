#include <rt/materials/combine.h>

namespace rt {

CombineMaterial::CombineMaterial()
{
    /* TODO */
}

void CombineMaterial::add(Material* m, float w) {
    materials.push_back(std::make_pair(m, w));
}

RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    RGBColor reflectance = RGBColor::rep(0.f);
    for (auto m : materials) {
        reflectance = reflectance + m.first->getReflectance(texPoint, normal, outDir, inDir) * m.second;
    }

    return reflectance;
}

RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    RGBColor emissions = RGBColor::rep(0.f);
    for (size_t i = 0; i < materials.size(); i++) {
        //assumed that sampling is not needed here.
        emissions = emissions + materials[i].first->getEmission(texPoint, normal, outDir) * materials[i].second;
    }

    return emissions;
}

Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    Sampling sampling;
    SampleReflectance sr;
    RGBColor reflectance = RGBColor::rep(0.f);
    Vector direction = Vector::rep(0.f);//No need to update direction, as reflection direction is dependent on these vectors and nor material
    for (auto m : materials) {
        //assumed that sampling is required here.
        sr = m.first->getSampleReflectance(texPoint, normal, outDir);
        reflectance = reflectance + sr.reflectance * m.second; // weight the reflectance.  
    }
    direction = sr.direction;
    return SampleReflectance(direction, reflectance);
}

Material::Sampling CombineMaterial::useSampling() const {
    return SAMPLING_SECONDARY;
}

}