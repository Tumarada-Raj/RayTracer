#include <rt/textures/imagetex.h>
#include <core/interpolate.h>

namespace rt {

    ImageTexture::ImageTexture()
    {
        this->bh = REPEAT;
        this->ip = BILINEAR;
    }

    ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i)
    {
        this->bh = bh;
        this->ip = i;
        Image image;
        image.readPNG(filename);
        this->img = image;
    }

    ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i)
    {
        this->bh = bh;
        this->ip = i;
        this->img = image;
    }

    RGBColor ImageTexture::getColor(const Point& coord) {
        float tu, tv, fu, fv, u, v;
        int llu = floor(coord.x);
        int llv = floor(coord.y);
        switch (bh) {
        case MIRROR:
            tu = coord.x - llu;
            tv = coord.y - llv;
            if (abs(llu) % 2 == 1)
                tu = 1 - tu;
            if (abs(llv) % 2 == 1)
                tv = 1 - tv;
            break;
        case CLAMP:
            tu = coord.x < 0 ? 0.0f : coord.x;
            tu = coord.x > 1 ? 1.0f : tu;
            tv = coord.y < 0 ? 0.0f : coord.y;
            tv = coord.y > 1 ? 1.0f : tv;
            break;
        default: //repeat
            tu = coord.x - llu;
            tv = coord.y - llv;
            break;
        }
        float h = img.height();
        float w = img.width();
        float lu, lv;
        switch (ip) {
        case NEAREST:
            u = tu * w;
            v = tv * h;
            lu = min(float(floor(u)), w - 1);
            lv = min(float(floor(v)), h - 1);
            return img(lu, lv);
            break;
        default: //bilinear
            u = tu * (1 - epsilon) * (w - 1.0f);
            v = tv * (1 - epsilon) * (h - 1.0f);
            fu = u - floor(u);
            fv = v - floor(v);
            u = floor(u);
            v = floor(v);
            //return (((1 - fu) * (1 - fv) * img(floor(u), floor(v))) +
            //    ((1 - fu) * (fv)*img(floor(u), floor(v) + 1)) +
            //    ((fu) * (1 - fv) * img(floor(u) + 1, floor(v))) +
            //    ((fu) * (fv)*img(floor(u) + 1, floor(v) + 1)));
            return lerp2d(img(u, v), img(u, v + 1), img(u + 1, v), img(u + 1, v + 1), fv, fu);
        }
    }

    const float& clamp(const float& v, const float& lo, const float& hi)
    {
        assert(!(hi < lo));
        return (v < lo) ? lo : (hi < v) ? hi : v;
    }

    RGBColor ImageTexture::getColorDX(const Point& coord) {
        RGBColor currentPixelColor = getColor(coord);
        float tu, tv, fu, fv, u, v;
        int llu = floor(coord.x);
        int llv = floor(coord.y);
        switch (bh) {
        case MIRROR:
            tu = coord.x - llu;
            tv = coord.y - llv;
            if (abs(llu) % 2 == 1)
                tu = 1 - tu;
            if (abs(llv) % 2 == 1)
                tv = 1 - tv;
            break;
        case CLAMP:
            tu = coord.x < 0 ? 0.0f : coord.x;
            tu = coord.x > 1 ? 1.0f : tu;
            tv = coord.y < 0 ? 0.0f : coord.y;
            tv = coord.y > 1 ? 1.0f : tv;
            break;
        default: //repeat
            tu = coord.x - llu;
            tv = coord.y - llv;
            break;
        }
        float h = img.height();
        float w = img.width();
        float lu, lv;
        switch (ip) {
        case NEAREST:
            u = tu * w;
            v = tv * h;
            lu = min(float(floor(u)), w - 1);
            lv = min(float(floor(v)), h - 1);
            return img(lu, lv) - img(clamp(lu + 1, 0, w - 1), lv);
            break;
        default: //bilinear
            u = tu * (1 - epsilon) * (w - 1.0f);
            v = tv * (1 - epsilon) * (h - 1.0f);
            fu = u - floor(u);
            fv = v - floor(v);

            RGBColor currentPixelColor = lerp2d(img(floor(u), floor(v)), img(floor(u), floor(v) + 1), img(floor(u) + 1, floor(v)), img(floor(u) + 1, floor(v) + 1), fv, fu);
            u = clamp(u + 1, 0, w - 2);
            RGBColor rightPixelColor = lerp2d(img(floor(u), floor(v)), img(floor(u), floor(v) + 1), img(floor(u) + 1, floor(v)), img(floor(u) + 1, floor(v) + 1), fv, fu);
            return currentPixelColor - rightPixelColor;
        } 

    }

    RGBColor ImageTexture::getColorDY(const Point& coord) {
        float tu, tv, fu, fv, u, v;
        int llu = floor(coord.x);
        int llv = floor(coord.y);
        switch (bh) {
        case MIRROR:
            tu = coord.x - llu;
            tv = coord.y - llv;
            if (abs(llu) % 2 == 1)
                tu = 1 - tu;
            if (abs(llv) % 2 == 1)
                tv = 1 - tv;
            break;
        case CLAMP:
            tu = coord.x < 0 ? 0.0f : coord.x;
            tu = coord.x > 1 ? 1.0f : tu;
            tv = coord.y < 0 ? 0.0f : coord.y;
            tv = coord.y > 1 ? 1.0f : tv;
            break;
        default: //repeat
            tu = coord.x - llu;
            tv = coord.y - llv;
            break;
        }
        float h = img.height();
        float w = img.width();
        float lu, lv;
        switch (ip) {
        case NEAREST:
            u = tu * w;
            v = tv * h;
            lu = min(float(floor(u)), w - 1);
            lv = min(float(floor(v)), h - 1);
            return img(lu, lv) - img(lu, clamp(lv + 1, 0, h - 1));
            break;
        default: //bilinear
            u = tu * (1 - epsilon) * (w - 1.0f);
            v = tv * (1 - epsilon) * (h - 1.0f);
            fu = u - floor(u);
            fv = v - floor(v);
            int flru = floor(u);
            int flrv = floor(v);
            RGBColor currentPixelColor = lerp2d(img(floor(u), floor(v)), img(floor(u), floor(v) + 1), img(floor(u) + 1, floor(v)), img(floor(u) + 1, floor(v) + 1), fv, fu);
            v = clamp(v + 1, 0, h - 2);
            RGBColor rightPixelColor = lerp2d(img(floor(u), floor(v)), img(floor(u), floor(v) + 1), img(floor(u) + 1, floor(v)), img(floor(u) + 1, floor(v) + 1), fv, fu);
            return currentPixelColor - rightPixelColor;


        }
    }

}