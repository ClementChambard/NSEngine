#include "./color.h"
#include "./math/math.h"

namespace ns {

Colorf::Colorf(Color c) {
  r = static_cast<float>(c.r) / 255.f;
  g = static_cast<float>(c.g) / 255.f;
  b = static_cast<float>(c.b) / 255.f;
  a = static_cast<float>(c.a) / 255.f;
}

Colorf Colorf::lerp(Colorf const& col_to, float w) const {
  Colorf res = *this;
  res.r += (w * (col_to.r - r));
  res.g += (w * (col_to.g - g));
  res.b += (w * (col_to.b - b));
  res.a += (w * (col_to.a - a));
  return res;
}

Colorf Colorf::darkened(float a) const {
  Colorf res = *this;
  res.r *= (1.0f - a);
  res.g *= (1.0f - a);
  res.b *= (1.0f - a);
  return res;
}

Colorf Colorf::lightened(float a) const {
  Colorf res = *this;
  res.r += (1.0f - res.r) * a;
  res.g += (1.0f - res.g) * a;
  res.b += (1.0f - res.b) * a;
  return res;
}

Colorf Colorf::blend(Colorf const& over) const {
  Colorf res;
  float sa = 1.0f - over.a;
  res.a = a * sa + over.a;
  if (res.a == 0) return Colorf{0, 0, 0, 0};
  else {
    res.r = (r * a * sa + over.r * over.a) / res.a;
    res.g = (g * a * sa + over.g * over.a) / res.a;
    res.b = (b * a * sa + over.b * over.a) / res.a;
  }
  return res;
}

u32 Colorf::to_abgr32() const {
    u32 c = static_cast<u8>(a * 255);
    c <<= 8;
    c |= static_cast<u8>(b * 255);
    c <<= 8;
    c |= static_cast<u8>(g * 255);
    c <<= 8;
    c |= static_cast<u8>(r * 255);
    return c;
}
u32 Colorf::to_argb32() const {
    u32 c = static_cast<u8>(a * 255);
    c <<= 8;
    c |= static_cast<u8>(r * 255);
    c <<= 8;
    c |= static_cast<u8>(g * 255);
    c <<= 8;
    c |= static_cast<u8>(b * 255);
    return c;
}
u32 Colorf::to_rgba32() const {
    u32 c = static_cast<u8>(r * 255);
    c <<= 8;
    c |= static_cast<u8>(g * 255);
    c <<= 8;
    c |= static_cast<u8>(b * 255);
    c <<= 8;
    c |= static_cast<u8>(a * 255);
    return c;
}

float Colorf::get_h() const {
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

float Colorf::get_s() const {
    float min_ = math::min(r, g);
    min_ = math::min(min_, b);
    float max_ = math::max(r, g);
    max_ = math::max(max_, b);

    float delta = max_ - min_;

    return (max_ != 0) ? (delta / max_) : 0;
}

float Colorf::get_v() const {
    float max_ = math::max(r, g);
    return math::max(max_, b);
}

Colorf& Colorf::set_hsv(float hh, float ss, float vv, float aa) {
    i32 i;
    float f, p, q, t;
    a = aa;

    if (ss == 0) {
        r = g = b = vv;
        return *this;
    }

    hh *= 6.0;
    hh = math::mod(hh, 6);
    i = (i32) floor(hh);

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
    return *this;
}

Colorf& Colorf::invert() {
    r = 1.f - r;
    g = 1.f - g;
    b = 1.f - b;
    return *this;
}

Colorf Colorf::inverted() const { Colorf c = *this; c.invert(); return c; }

Colorf Colorf::from_rgba32(u32 hex_num) {
    float a = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float b = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float g = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float r = (hex_num & 0xFF) / 255.f;

    return {r,g,b,a};
}

Colorf Colorf::from_argb32(u32 hex_num) {
    float b = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float g = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float r = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float a = (hex_num & 0xFF) / 255.f;

    return {r,g,b,a};
}

Colorf Colorf::from_abgr32(u32 hex_num) {
    float r = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float g = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float b = (hex_num & 0xFF) / 255.f;
    hex_num >>= 8;
    float a = (hex_num & 0xFF) / 255.f;

    return {r,g,b,a};
}

Colorf Colorf::operator+(const Colorf &col) const {
    return {r + col.r, g + col.g, b + col.b, a + col.a};
}

Colorf& Colorf::operator+=(const Colorf &p_color) {
    r = r + p_color.r;
    g = g + p_color.g;
    b = b + p_color.b;
    a = a + p_color.a;
    return *this;
}

Colorf Colorf::operator-(const Colorf &p_color) const {
    return {r - p_color.r, g - p_color.g, b - p_color.b, a - p_color.a};
}

Colorf& Colorf::operator-=(const Colorf &p_color) {
    r = r - p_color.r;
    g = g - p_color.g;
    b = b - p_color.b;
    a = a - p_color.a;
    return *this;
}

Colorf Colorf::operator*(const Colorf &p_color) const {
    return {r * p_color.r, g * p_color.g, b * p_color.b, a * p_color.a};
}

Colorf Colorf::operator*(float p_scalar) const {
    return {r * p_scalar, g * p_scalar, b * p_scalar, a * p_scalar};
}

Colorf& Colorf::operator*=(const Colorf &p_color) {
    r = r * p_color.r;
    g = g * p_color.g;
    b = b * p_color.b;
    a = a * p_color.a;
    return *this;
}

Colorf& Colorf::operator*=(float p_scalar) {
    r = r * p_scalar;
    g = g * p_scalar;
    b = b * p_scalar;
    a = a * p_scalar;
    return *this;
}

Colorf Colorf::operator/(const Colorf &p_color) const {
    return {r / p_color.r, g / p_color.g, b / p_color.b, a / p_color.a};
}

Colorf Colorf::operator/(float p_scalar) const {
    return {r / p_scalar, g / p_scalar, b / p_scalar, a / p_scalar};
}

 Colorf&Colorf::operator/=(const Colorf &p_color) {
    r = r / p_color.r;
    g = g / p_color.g;
    b = b / p_color.b;
    a = a / p_color.a;
    return *this;
}

Colorf& Colorf::operator/=(float p_scalar) {
    r = r / p_scalar;
    g = g / p_scalar;
    b = b / p_scalar;
    a = a / p_scalar;
    return *this;
}

Colorf Colorf::operator-() const {
    return {1.f - r, 1.f - g, 1.f - b, 1.f - a};
}

Colorf Colorf::operator+() const {
    return *this;
}

//////////////////  COLOR  //////////////////////

Color Color::lerp(Color const& col_to, float w) const {
  Color res = *this;
  res.r += (w * (col_to.r - r));
  res.g += (w * (col_to.g - g));
  res.b += (w * (col_to.b - b));
  res.a += (w * (col_to.a - a));
  return res;
}

Color Color::darkened(float a) const {
  Color res = *this;
  res.r *= (1.0f - a);
  res.g *= (1.0f - a);
  res.b *= (1.0f - a);
  return res;
}

Color Color::lightened(float a) const {
  Color res = *this;
  res.r += (255 - res.r) * a;
  res.g += (255 - res.g) * a;
  res.b += (255 - res.b) * a;
  return res;
}

Color Color::blend(Color const& over) const {
  Color res;
  float sa = 1.0f - over.a / 255.f;
  float resa = a * sa + over.a;
  if (resa == 0) return Color{0, 0, 0, 0};
  else {
    res.r = (r * a * sa + over.r * over.a) / (resa * 255);
    res.g = (g * a * sa + over.g * over.a) / (resa * 255);
    res.b = (b * a * sa + over.b * over.a) / (resa * 255);
  }
  res.a = static_cast<u8>(resa);
  return res;
}

u32 Color::to_abgr32() const {
    u32 c = a;
    c <<= 8;
    c |= b;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= r;
    return c;
}
u32 Color::to_argb32() const {
    u32 c = a;
    c <<= 8;
    c |= r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
}
u32 Color::to_rgba32() const {
    u32 c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    c <<= 8;
    c |= a;
    return c;
}

Color& Color::invert() {
    r = 1.f - r;
    g = 1.f - g;
    b = 1.f - b;
    return *this;
}

Color Color::inverted() const { Color c = *this; return c.invert(); }

Color Color::from_rgba32(u32 hex_num) {
    u8 a = (hex_num & 0xFF);
    hex_num >>= 8;
    u8 b = (hex_num & 0xFF);
    hex_num >>= 8;
    u8 g = (hex_num & 0xFF);
    hex_num >>= 8;
    u8 r = (hex_num & 0xFF);

    return {r,g,b,a};
}

Color Color::from_argb32(u32 hex_num) {
    u8 b = (hex_num & 0xFF);
    hex_num >>= 8;
    u8 g = (hex_num & 0xFF);
    hex_num >>= 8;
    u8 r = (hex_num & 0xFF);
    hex_num >>= 8;
    u8 a = (hex_num & 0xFF);

    return {r,g,b,a};
}

Color Color::from_abgr32(u32 hex_num) {
    u8 r = (hex_num & 0xFF);
    hex_num >>= 8;
    u8 g = (hex_num & 0xFF);
    hex_num >>= 8;
    u8 b = (hex_num & 0xFF);
    hex_num >>= 8;
    u8 a = (hex_num & 0xFF);

    return {r,g,b,a};
}

static inline u8 u8add(u8 i1, u8 i2) {
    return (static_cast<i32>(i1) + static_cast<i32>(i2) > 255) ? 255 : i1 + i2;
}

Color Color::operator+(const Color &col) const {
    return {
        u8add(r, col.r),
        u8add(g, col.g),
        u8add(b, col.b),
        u8add(a, col.a)
    };
}

Color& Color::operator+=(const Color &p_color) {
    r = u8add(r, p_color.r);
    g = u8add(r, p_color.g);
    b = u8add(r, p_color.b);
    a = u8add(r, p_color.a);
    return *this;
}

static inline u8 u8sub(u8 i1, u8 i2) {
    return (static_cast<i32>(i1) - static_cast<i32>(i2) < 0) ? 0 : i1 - i2;
}

Color Color::operator-(const Color &p_color) const {
    return {
        u8sub(r, p_color.r),
        u8sub(g, p_color.g),
        u8sub(b, p_color.b),
        u8sub(a, p_color.a)
    };
}

Color& Color::operator-=(const Color &p_color) {
    r = u8sub(r, p_color.r);
    g = u8sub(g, p_color.g);
    b = u8sub(b, p_color.b);
    a = u8sub(a, p_color.a);
    return *this;
}

static inline u8 u8mul(u8 i1, u8 i2) {
    return (static_cast<i32>(i1) * static_cast<i32>(i2) > 255) ? 255 : i1 * i2;
}
static inline u8 u8mul(u8 i1, f32 f2) {
    float m = i1 * f2;
    if (m < 0) return 0;
    if (m > 255) return 255;
    return static_cast<u8>(m);
}

Color Color::operator*(const Color &p_color) const {
    return {
        u8mul(r, p_color.r),
        u8mul(g, p_color.g),
        u8mul(b, p_color.b),
        u8mul(a, p_color.a)
    };
}

Color Color::operator*(float p_scalar) const {
    return {
        u8mul(r, p_scalar),
        u8mul(g, p_scalar),
        u8mul(b, p_scalar),
        u8mul(a, p_scalar)
    };
}

Color& Color::operator*=(const Color &p_color) {
    r = u8mul(r, p_color.r);
    g = u8mul(g, p_color.g);
    b = u8mul(b, p_color.b);
    a = u8mul(a, p_color.a);
    return *this;
}

Color& Color::operator*=(float p_scalar) {
    r = u8mul(r, p_scalar);
    g = u8mul(g, p_scalar);
    b = u8mul(b, p_scalar);
    a = u8mul(a, p_scalar);
    return *this;
}

static inline u8 u8div(u8 i1, u8 i2) {
    return i1 / i2;
}
static inline u8 u8div(u8 i1, f32 f2) {
    float m = i1 / f2;
    if (m < 0) return 0;
    if (m > 255) return 255;
    return static_cast<u8>(m);
}

Color Color::operator/(const Color &p_color) const {
    return {
        u8div(r, p_color.r),
        u8div(g, p_color.g),
        u8div(b, p_color.b),
        u8div(a, p_color.a)
    };
}

Color Color::operator/(float p_scalar) const {
    return {
        u8div(r, p_scalar),
        u8div(g, p_scalar),
        u8div(b, p_scalar),
        u8div(a, p_scalar)
    };
}

 Color&Color::operator/=(const Color &p_color) {
    r = u8div(r, p_color.r);
    g = u8div(g, p_color.g);
    b = u8div(b, p_color.b);
    a = u8div(a, p_color.a);
    return *this;
}

Color& Color::operator/=(float p_scalar) {
    r = u8div(r, p_scalar);
    g = u8div(g, p_scalar);
    b = u8div(b, p_scalar);
    a = u8div(a, p_scalar);
    return *this;
}

} // namespace ns

