#include <rt/materials/mirror.h>
#include <core/scalar.h>

namespace rt {

MirrorMaterial::MirrorMaterial(float eta, float kappa)
{
    this->eta = eta;
    this->kappa = kappa;
}

RGBColor MirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    float cosIn = dot(-inDir, normal);
    float cosOut = dot(outDir, normal);
    float delta = fabs(cosIn - cosOut) == 0.f ? 1.f : 0.f;

    float denom = 1.f / (kappa * cosIn + eta * cosOut);
    float r1 = (kappa * cosIn - eta * cosOut) * denom;
    float r2 = (eta * cosIn - kappa * cosOut) * denom;
    float Fr = 0.5f * (r1 + r2);
    float brdf = Fr * delta / fabs(cosIn);

    return RGBColor::rep(0.0f);
}

RGBColor MirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0.f);
}

Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    float eta_kappa_sqr = (eta * eta) + (kappa * kappa);
    float two_eta = 2 * eta;
    Vector ref_dir = -outDir + (2 * dot(outDir, normal) * normal);
    ref_dir = ref_dir.normalize();

    float cos_i = dot(ref_dir, normal);
    float cos_i_sq = cos_i * cos_i;
    float two_n_cos_theta = two_eta * cos_i;

    float r_perp = eta_kappa_sqr - two_n_cos_theta + cos_i_sq;
    r_perp = r_perp / (eta_kappa_sqr + two_n_cos_theta + cos_i_sq);

    float r_para = (eta_kappa_sqr * cos_i_sq) - two_n_cos_theta + 1;
    r_para = r_para / ((eta_kappa_sqr * cos_i_sq) + two_n_cos_theta + 1);

    return SampleReflectance(ref_dir, RGBColor::rep(0.5f * (r_para + r_perp)));
}

Material::Sampling MirrorMaterial::useSampling() const {
    return SAMPLING_ALL;
}

}