#include <rt/materials/fuzzymirror.h>
#include <rt/solids/disc.h>

namespace rt {

FuzzyMirrorMaterial::FuzzyMirrorMaterial(float eta, float kappa, float fuzzyangle)
{
    this->eta = eta;
    this->kappa = kappa;
    this->fuzzyAngle = fuzzyangle;
}

RGBColor FuzzyMirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    return RGBColor::rep(0.f);
}

RGBColor FuzzyMirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.f);
}

Material::SampleReflectance FuzzyMirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    Vector perfectReflection = (-outDir + 2.f * dot(outDir, normal) * normal).normalize();

    Disc disc(texPoint + perfectReflection, perfectReflection, tan(fuzzyAngle), nullptr, nullptr);

    Vector direction = disc.sample().point - texPoint;

    // Slide # 46.
    float eta_kappa_sqr = eta * eta + kappa * kappa;
    float cosIn = dot(normal, direction);
    float cosInsq = cosIn * cosIn;
    float two_eta_cosIn = 2.f * eta * cosIn;

    float rParallel = (eta_kappa_sqr * cosInsq - two_eta_cosIn + 1) /
        (eta_kappa_sqr * cosInsq + two_eta_cosIn + 1);

    float rPerpendicular = (eta_kappa_sqr - two_eta_cosIn + cosInsq) /
        (eta_kappa_sqr + two_eta_cosIn + cosInsq);

    float Fr = 0.5f * (rParallel + rPerpendicular);
    return SampleReflectance(direction.normalize(), RGBColor::rep(Fr));
}

Material::Sampling FuzzyMirrorMaterial::useSampling() const {
    /* TODO */
    return SAMPLING_ALL;
}

}
