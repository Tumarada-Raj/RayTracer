#include <rt/materials/glass.h>
#include <core/random.h>
#include <core/scalar.h>
#include <cmath>


namespace rt {

    GlassMaterial::GlassMaterial(float eta)
    {
        this->eta = eta;
    }

    RGBColor GlassMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
        /* TODO */
        return RGBColor::rep(0.f);
    }

    RGBColor GlassMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        /* TODO */
        return RGBColor::rep(0.f);
    }


    Vector GlassMaterial::refract(const Vector I, const Vector& N, const float& ior) const
    {
        float cosi = dot(I, N);
        float etai = 1, etat = ior;
        Vector n = N;
        if (cosi < 0) { cosi = -cosi; }
        else { std::swap(etai, etat); n = -N; }
        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);
        if (k < 0) {
            return Vector::rep(0.f);
        }
        else
            return eta * I + (eta * cosi - sqrtf(k)) * n;
    }

    float GlassMaterial::fresnel(const Vector I, const Vector& N, const float& ior) const
    {
        float kr;
        float cosi = dot(I, N);
        float etai = 1, etat = ior;
        if (cosi > 0) { std::swap(etai, etat); }
        // Computed using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        
        // Total internal reflection
        if (sint >= 1) 
            kr = 1;
        else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
        // As a consequence of the conservation of energy, transmittance is given by:
        // kt = 1 - kr;
        return kr;
    }

    Material::SampleReflectance GlassMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
        /* TODO */

        Vector reflDir = (-outDir + (2 * dot(outDir, normal) * normal)).normalize();
        Vector refraDir = refract(-outDir, normal, eta);
        float kr = fresnel(-outDir, normal, eta);

        if (kr >= 1)
            return SampleReflectance(reflDir.normalize(), RGBColor::rep(1.f));
        else if (random() < 0.5f)
            return SampleReflectance(reflDir.normalize(), 2 * RGBColor::rep(kr));
        else {
            float cosi = dot(-outDir, normal);
            float etai = 1, etat = eta;
            if (cosi > 0)
                std::swap(etai, etat);
            float inverse_eta = etai / etat;
            return SampleReflectance(refraDir.normalize(), 2 * sqr(inverse_eta) * RGBColor::rep((1.f - kr)));
        }
    }

    Material::Sampling GlassMaterial::useSampling() const {
        /* TODO */
        return SAMPLING_ALL;
    }

}