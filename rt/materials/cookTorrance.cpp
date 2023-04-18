#include <rt/materials/cookTorrance.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>
#include <cmath>
#include <algorithm>


namespace rt {
    CookTorrance::CookTorrance(rt::Texture* glossy, rt::Texture* diffuse, float kappa, float eta) {
        this->glossy = glossy;
        this->diffuse = diffuse;
        this->kappa = kappa;
        this->eta = eta;
    }

    RGBColor CookTorrance::getReflectance(const rt::Point& texPoint, const rt::Vector& normal, const rt::Vector& outDir, const rt::Vector& inDir) const {
        // lambertian: duffise.
        return (1.f / pi) * dot(normal, -inDir) * diffuse->getColor(texPoint);
    }

    RGBColor CookTorrance::getEmission(const rt::Point& texPoint, const rt::Vector& normal, const rt::Vector& outDir) const {
        return RGBColor::rep(0.f);
    }

    Material::SampleReflectance CookTorrance::getSampleReflectance(const rt::Point& texPoint, const rt::Vector& normal, const rt::Vector& outDir) const {
        // return mirror sample reflectance.
        Vector refDir = -outDir + 2.f * dot(outDir, normal) * normal;
        //TODO: Fr and D are = 0
        RGBColor Fr = getFresnel(normal, refDir, outDir);
        float G = getGeometricAttenuation(normal, refDir, outDir);
        float D = getMicrofacetDistribution(normal, refDir, outDir);
        RGBColor kGlossy = Fr * D * G / (pi * dot(normal, outDir) * dot(normal, refDir));
        RGBColor kDiffuse = (1.f / pi) * dot(normal, refDir) * diffuse->getColor(texPoint);
        // glossy and diffuse constants. should sum up to one. //TODO; change later, just a guess for now.
        float Pd = 0.5f, Pg = 0.5f;
        RGBColor Brdf = kGlossy * Pg + kDiffuse * Pd;

        SampleReflectance ref;
        ref.direction = refDir;
        ref.reflectance = Brdf;
        return ref;
    }

    Material::Sampling CookTorrance::useSampling() const {
        return SAMPLING_SECONDARY;
    }

    RGBColor CookTorrance::getFresnel(const Vector& normal, const Vector& inDir, const Vector& outDir) const {
        //copied from mirror reflectance function.
        float cosThetaIn = dot(inDir, normal);
        //compute refracted dir.
        Vector ref_dir = -outDir + (2 * dot(outDir, normal) * normal);
        ref_dir = ref_dir.normalize();
        float cosThetaOut = dot(ref_dir, normal);

        float delta = fabs(cosThetaIn - cosThetaOut) == 0.f ? 1.f : 0.f;

        float denom = 1.f / (kappa * cosThetaIn + eta * cosThetaOut);
        float r1 = (kappa * cosThetaIn - eta * cosThetaOut) * denom;
        float r2 = (eta * cosThetaIn - kappa * cosThetaOut) * denom;
        float Fr = 0.5f * (r1 + r2);
        float brdf = Fr * delta / fabs(cosThetaIn);

        return RGBColor::rep(brdf);
    }

    float CookTorrance::getGeometricAttenuation(const rt::Vector& normal, const rt::Vector& inDir, const rt::Vector& outDir) const {
        Vector H = (inDir + outDir).normalize();
        float gReflected = 2.f * dot(normal, H) * dot(normal, outDir) / dot(outDir, H);
        float gIncident = 2.f * dot(normal, H) * dot(normal, inDir) / dot(outDir, H);

        return std::min(1.f, min(gReflected, gIncident));
    }

    float CookTorrance::getMicrofacetDistribution(const rt::Vector& normal, const rt::Vector& inDir, const rt::Vector& outDir) const {
        // found a formula for H at:
        //https://computergraphics.stackexchange.com/questions/4394/path-tracing-the-cook-torrance-brdf
        Vector H = (inDir + outDir).normalize();
        float theta = dot(normal.normalize(), H);
        float x = (tan(theta) / m, 2);
        float D = (1.f / (pi * m * m * pow(cos(theta), 4))) * exp(-x);
        return D;
    }

}