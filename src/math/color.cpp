#include "color.h"
#include "math.h"

unsigned int Color::to_abgr32() const {
    unsigned int c = (unsigned char)(a * 255);
    c <<= 8;
    c |= (unsigned char)(b * 255);
    c <<= 8;
    c |= (unsigned char)(g * 255);
    c <<= 8;
    c |= (unsigned char)(r * 255);
    return c;
}
unsigned int Color::to_argb32() const {
    unsigned int c = (unsigned char)(a * 255);
    c <<= 8;
    c |= (unsigned char)(r * 255);
    c <<= 8;
    c |= (unsigned char)(g * 255);
    c <<= 8;
    c |= (unsigned char)(b * 255);
    return c;
}
unsigned int Color::to_rgba32() const {
    unsigned int c = (unsigned char)(r * 255);
    c <<= 8;
    c |= (unsigned char)(g * 255);
    c <<= 8;
    c |= (unsigned char)(b * 255);
    c <<= 8;
    c |= (unsigned char)(a * 255);
    return c;
}

float Color::get_h() const {
    float min_ = math::min(r, g);
    min_ = math::min(min_, b);
    float max_ = math::max(r, g);
    max_ = math::max(max_, b);

    float delta = max_ - min_;

    if (delta == 0) {
        return 0;
    }

    float h;
    if (r == max_) {
        h = (g - b) / delta;
    } else if (g == max_) {
        h = 2 + (b - r) / delta;
    } else {
        h = 4 + (r - g) / delta;
    }

    h /= 6.0;
    if (h < 0) h += 1.0;

    return h;
}

float Color::get_s() const {
    float min_ = math::min(r, g);
    min_ = math::min(min_, b);
    float max_ = math::max(r, g);
    max_ = math::max(max_, b);

    float delta = max_ - min_;

    return (max_ != 0) ? (delta / max_) : 0;
}

float Color::get_v() const {
    float max_ = math::max(r, g);
    return math::max(max_, b);
}

void Color::set_hsv(float hh, float ss, float vv, float aa) {
    int i;
    float f, p, q, t;
    a = aa;

    if (ss == 0) {
        r = g = b = vv;
        return;
    }

    hh *= 6.0;
    hh = math::mod(hh, 6);
    i = (int) floor(hh);

    f = hh - i;
    p = vv * (1-ss);
    q = vv * (1-ss*f);
    t = vv * (1-ss*(1-f));

    switch (i) {
        case 0:
            r = vv;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = vv;
            b = p;
            break;
        case 2:
            r = p;
            g = vv;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = vv;
            break;
        case 4:
            r = t;
            g = p;
            b = vv;
            break;
        default:
            r = vv;
            g = p;
            b = q;
            break;
    }
}

void Color::invert() {
    r = 1.f - r;
    g = 1.f - g;
    b = 1.f - b;
}

Color Color::inverted() const { Color c = *this; c.invert(); return c; }

Color Color::from_int(int hex_num)
{
    float a = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float b = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float g = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float r = (hex_num & 0xFF) / 255.f;

    return {r,g,b,a};
}

Color Color::operator+(const Color &col) const {
    return {r + col.r, g + col.g, b + col.b, a + col.a};
}

void Color::operator+=(const Color &p_color) {
    r = r + p_color.r;
    g = g + p_color.g;
    b = b + p_color.b;
    a = a + p_color.a;
}

Color Color::operator-(const Color &p_color) const {
    return {r - p_color.r, g - p_color.g, b - p_color.b, a - p_color.a};
}

void Color::operator-=(const Color &p_color) {
    r = r - p_color.r;
    g = g - p_color.g;
    b = b - p_color.b;
    a = a - p_color.a;
}

Color Color::operator*(const Color &p_color) const {
    return {r * p_color.r, g * p_color.g, b * p_color.b, a * p_color.a};
}

Color Color::operator*(float p_scalar) const {
    return {r * p_scalar, g * p_scalar, b * p_scalar, a * p_scalar};
}

void Color::operator*=(const Color &p_color) {
    r = r * p_color.r;
    g = g * p_color.g;
    b = b * p_color.b;
    a = a * p_color.a;
}

void Color::operator*=(float p_scalar) {
    r = r * p_scalar;
    g = g * p_scalar;
    b = b * p_scalar;
    a = a * p_scalar;
}

Color Color::operator/(const Color &p_color) const {
    return {r / p_color.r, g / p_color.g, b / p_color.b, a / p_color.a};
}

Color Color::operator/(float p_scalar) const {
    return {r / p_scalar, g / p_scalar, b / p_scalar, a / p_scalar};
}

void Color::operator/=(const Color &p_color) {
    r = r / p_color.r;
    g = g / p_color.g;
    b = b / p_color.b;
    a = a / p_color.a;
}

void Color::operator/=(float p_scalar) {
    r = r / p_scalar;
    g = g / p_scalar;
    b = b / p_scalar;
    a = a / p_scalar;
}

Color Color::operator-() const {
    return {1.f - r, 1.f - g, 1.f - b, 1.f - a};
}
