#include "saturate.h"

#include <iostream>

Pixel HsvToRgb(HsvColor hsv)  // NOLINT
{
    Pixel rgb;
    Color region, remainder, p, q, t;

    if (hsv.s == 0) {
        rgb.red = hsv.v;
        rgb.green = hsv.v;
        rgb.blue = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:
            rgb.red = hsv.v;
            rgb.green = t;
            rgb.blue = p;
            break;
        case 1:
            rgb.red = q;
            rgb.green = hsv.v;
            rgb.blue = p;
            break;
        case 2:
            rgb.red = p;
            rgb.green = hsv.v;
            rgb.blue = t;
            break;
        case 3:
            rgb.red = p;
            rgb.green = q;
            rgb.blue = hsv.v;
            break;
        case 4:
            rgb.red = t;
            rgb.green = p;
            rgb.blue = hsv.v;
            break;
        default:
            rgb.red = hsv.v;
            rgb.green = p;
            rgb.blue = q;
            break;
    }

    return rgb;
}

HsvColor RgbToHsv(Pixel rgb)  // NOLINT
{
    HsvColor hsv;
    Color rgbMin, rgbMax;

    rgbMin = std::min(std::min(rgb.red, rgb.blue), rgb.green);
    rgbMax = std::max(std::max(rgb.red, rgb.blue), rgb.green);

    hsv.v = rgbMax;
    if (hsv.v == 0) {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0) {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.red)
        hsv.h = 0 + 43 * (rgb.green - rgb.blue) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.green)
        hsv.h = 85 + 43 * (rgb.blue - rgb.red) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.red - rgb.green) / (rgbMax - rgbMin);

    return hsv;
}

void SaturateFilter::operator()(Image &image) const {
    for (auto &line : image.colors_) {
        for (auto &pixel : line) {
            auto hsv = RgbToHsv(pixel);
            if (add) {
                hsv.s += saturation_delta;
                // std::cout << (int)(hsv.s) << '\n';
                hsv.s = std::min(Pixel::PixelMax, hsv.s);
            } else {
                hsv.s = hsv.s >= saturation_delta ? hsv.s - saturation_delta : 0;
            }
            pixel = HsvToRgb(hsv);
        }
    }
}

SaturateFilter::SaturateFilter(bool plus, std::size_t delta) : add(plus), saturation_delta(delta) {
}

SaturateFilter::~SaturateFilter() noexcept {
}
