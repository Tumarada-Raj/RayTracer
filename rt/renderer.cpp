#include <core/scalar.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <rt/cameras/camera.h>
#include <rt/ray.h>
#include <iostream>
#include <rt/integrators/integrator.h>
#include <core/random.h>

namespace rt {

    Renderer::Renderer(Camera* cam, Integrator* integrator)
        : cam(cam), integrator(integrator), samples(1)
    {}

    /*void Renderer::render(Image& img) {
        Ray ray;
        float ndcx, ndcy, sscx, sscy;
        int i, j;   
        //float aspect_ratio = img.width() / img.height();
    //#pragma omp parallel for
        for (i = 0; i < img.width(); i++)
            for (j = 0; j < img.height(); j++) {
                
                if (samples == 1) {
                    //Took reference from slides for normalizing device coordinates [0,1]
                    float ndcx = (i + 0.5) / img.width();
                    float ndcy = (j + 0.5) / img.height();

                    //Took reference from slides for Screen Space coordinates [-1,1]
                    float sscx = ndcx * 2 - 1;
                    float sscy = 1 - ndcy * 2;

                    ray = (this->cam)->getPrimaryRay(sscx, sscy);
                    RGBColor pixelColor = (this->integrator)->getRadiance(ray);
                    img(i, j) = pixelColor;
                }
                else {
                    RGBColor pixcelColorSum = RGBColor::rep(0.f);
                    for (int s = 0; s < samples; s++) {
                        ndcx = (i + random(0,1)) / img.width(); //Don't shoot through the pixel center.
                        ndcy = (j + random(0,1))/ img.height();
                        // Screen space coordinates [-1, 1]
                        sscx = (ndcx * 2 - 1);
                        sscy = 1 - ndcy * 2;
                        ray = (this->cam)->getPrimaryRay(sscx, sscy);
                        pixcelColorSum = pixcelColorSum + (this->integrator)->getRadiance(ray);
                    }
                    img(i, j) = pixcelColorSum / float(samples);
                }
            }
        }*/

    void Renderer::render(Image& img) {
        auto img_w = img.width(), img_h = img.height();

        auto aspect_ratio = img_w / img_h;

        #pragma omp parallel for
        for (long long iter = 0; iter < (img_w * img_h); ++iter) {
            auto x = iter / img_h;
            auto y = iter % img_h;

            RGBColor pixelColor = RGBColor::rep(0);

            // #pragma omp parallel for
            for (int i = 0; i < samples; ++i) {
                float dx = 0.5;
                float dy = 0.5;

                if (i != 0) {
                    dx = random(0,1);
                    dy = random(0,1);
                }

                auto ndc_x = (x + dx) / img_w;
                auto ndc_y = (y + dy) / img_h;

                // auto ssx = (1 - 2 * ndc_x) * aspect_ratio; // blender mode

                auto ssx = (2 * ndc_x - 1) * aspect_ratio;
                auto ssy = 1 - 2 * ndc_y;

                auto ray = cam->getPrimaryRay(ssx, ssy); // primary ray
                pixelColor = pixelColor + integrator->getRadiance(ray);     // imagePlane 
            }
            img(x, y) = pixelColor / samples;
        }
    }
}

rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);

namespace rt {

void Renderer::test_render1(Image& img) {
    for (int i = 0; i < img.width(); i++) {
        for (int j = 0; j < img.height(); j++) {
            img(i, j) = a1computeColor(i, j, img.width(), img.height());
        }
    }
}
}

rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt {

void Renderer::test_render2(Image& img) {
    float aspect_ratio = img.width() / img.height();
    for (int i = 0; i < img.width(); i++) {
        for (int j = 0; j < img.height(); j++) {

            //Took reference from slides for normalizing device coordinates [0,1]
            float ndcx = (i + 0.5) / img.width();
            float ndcy = (j + 0.5) / img.height();

            //Took reference from slides for Screen Space coordinates [-1,1]
            float sscx = (ndcx * 2 - 1) * aspect_ratio;
            float sscy = 1 - ndcy * 2;

            img(i, j) = a2computeColor(cam->getPrimaryRay(sscx, sscy));
        }
    }
}

}
