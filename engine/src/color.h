#ifndef COLOR_HEADER_INCLUDED
#define COLOR_HEADER_INCLUDED

#include "./defines.h"

namespace ns {

struct Color;

struct Colorf {
    union {
        f32 components[4] = { 0, 0, 0, 1.0 };
        struct {
            f32 r;
            f32 g;
            f32 b;
            f32 a;
        };
    };
    Colorf() = default;
    Colorf(f32 gray, f32 a = 1.0) : Colorf(gray, gray, gray, a) {}
    Colorf(f32 r, f32 g, f32 b, f32 a = 1.0) : r(r), g(g), b(b), a(a) {}
    Colorf(Color c);
    u32 to_rgba32() const;
    u32 to_argb32() const;
    u32 to_abgr32() const;
    static Colorf from_rgba32(u32 n);
    static Colorf from_argb32(u32 n);
    static Colorf from_abgr32(u32 n);
    Color to_color() const;
    f32 get_h() const;
    f32 get_s() const;
    f32 get_v() const;
    Colorf& set_hsv(f32 h, f32 s, f32 v, f32 a = 1.f);
    u8 get_r() const { return static_cast<u8>(r * 255); }
    u8 get_g() const { return static_cast<u8>(g * 255); }
    u8 get_b() const { return static_cast<u8>(b * 255); }
    u8 get_a() const { return static_cast<u8>(a * 255); }
    Colorf& set_r(u8 r) { this->r = static_cast<f32>(r) / 255.f; return *this; }
    Colorf& set_g(u8 g) { this->g = static_cast<f32>(g) / 255.f; return *this; }
    Colorf& set_b(u8 b) { this->b = static_cast<f32>(b) / 255.f; return *this; }
    Colorf& set_a(u8 a) { this->a = static_cast<f32>(a) / 255.f; return *this; }
    Colorf& set_h(f32 h) { return set_hsv(h, get_s(), get_v()); }
    Colorf& set_s(f32 s) { return set_hsv(get_h(), s, get_v()); }
    Colorf& set_v(f32 v) { return set_hsv(get_h(), get_s(), v); }

    Colorf& invert();
    Colorf inverted() const;

    Colorf lerp(Colorf const& col_to, f32 w) const;
    Colorf darkened(f32 amount) const;
    Colorf lightened(f32 amount) const;
    Colorf blend(Colorf const& over) const;

    f32& operator[](usize i) { return components[i]; }
    f32  operator[](usize i) const { return components[i]; }

    bool operator==(Colorf const& other) const {
        return r == other.r && g == other.g
            && b == other.b && a == other.a;
    }
    
    Colorf  operator+() const;
    Colorf  operator-() const;
    Colorf  operator+(Colorf const& col) const;
    Colorf& operator+=(Colorf const& col);
    Colorf  operator-(Colorf const& col) const;
    Colorf& operator-=(Colorf const& col);
    Colorf  operator*(Colorf const& col) const;
    Colorf  operator*(f32 scalar) const;
    Colorf& operator*=(Colorf const& col);
    Colorf& operator*=(f32 scalar);
    Colorf  operator/(Colorf const& col) const;
    Colorf  operator/(f32 scalar) const;
    Colorf& operator/=(Colorf const& col);
    Colorf& operator/=(f32 scalar);
};

inline Colorf operator*(f32 scalar, Colorf const& col) { return col * scalar; }




struct Color {
    union {
        u8 components[4] = { 0, 0, 0, 255 };
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };
    };
    Color() = default;
    Color(u8 gray, u8 a = 255) : Color(gray, gray, gray, a) {}
    Color(u8 r, u8 g, u8 b, u8 a = 255) : r(r), g(g), b(b), a(a) {}
    Color(Colorf c) {
        r = static_cast<u8>(c.r * 255);
        g = static_cast<u8>(c.g * 255);
        b = static_cast<u8>(c.b * 255);
        a = static_cast<u8>(c.a * 255);
    }
    u32 to_rgba32() const;
    u32 to_argb32() const;
    u32 to_abgr32() const;
    static Color from_rgba32(u32 n);
    static Color from_argb32(u32 n);
    static Color from_abgr32(u32 n);
    Colorf to_colorf() const { return Colorf(*this); }
    f32 get_h() const { return Colorf(*this).get_h(); }
    f32 get_s() const { return Colorf(*this).get_s(); }
    f32 get_v() const { return Colorf(*this).get_v(); }
    Color& set_hsv(f32 h, f32 s, f32 v, f32 a = 1.f) { *this = Colorf(*this).set_hsv(h, s, v, a); return *this; }
    f32 get_r() const { return static_cast<f32>(r) / 255.f; }
    f32 get_g() const { return static_cast<f32>(g) / 255.f; }
    f32 get_b() const { return static_cast<f32>(b) / 255.f; }
    f32 get_a() const { return static_cast<f32>(a) / 255.f; }
    Color& set_r(f32 r) { this->r = static_cast<u8>(r * 255); return *this; }
    Color& set_g(f32 g) { this->g = static_cast<u8>(g * 255); return *this; }
    Color& set_b(f32 b) { this->b = static_cast<u8>(b * 255); return *this; }
    Color& set_a(f32 a) { this->a = static_cast<u8>(a * 255); return *this; }
    Color& set_h(f32 h) { *this = Colorf(*this).set_h(h); return *this; }
    Color& set_s(f32 s) { *this = Colorf(*this).set_s(s); return *this; }
    Color& set_v(f32 v) { *this = Colorf(*this).set_v(v); return *this; }

    Color& invert();
    Color inverted() const;

    Color lerp(Color const& col_to, f32 w) const;
    Color darkened(f32 amount) const;
    Color lightened(f32 amount) const;
    Color blend(Color const& over) const;

    u8& operator[](usize i) { return components[i]; }
    u8  operator[](usize i) const { return components[i]; }

    bool operator==(Colorf const& other) const {
        return r == other.r && g == other.g
            && b == other.b && a == other.a;
    }

    Color  operator+(Color const& col) const;
    Color& operator+=(Color const& col);
    Color  operator-(Color const& col) const;
    Color& operator-=(Color const& col);
    Color  operator*(Color const& col) const;
    Color  operator*(f32 scalar) const;
    Color& operator*=(Color const& col);
    Color& operator*=(f32 scalar);
    Color  operator/(Color const& col) const;
    Color  operator/(f32 scalar) const;
    Color& operator/=(Color const& col);
    Color& operator/=(f32 scalar);
};

inline Color operator*(f32 scalar, Color const& col) { return col * scalar; }


inline Color mix(Color const& c1, Color const& c2) {
    Color out;
    int tmp;
    tmp = c1.r * c2.r / 255;
    if (tmp > 255) out.r = 255;
    else           out.r = tmp;
    tmp = c1.g * c2.g / 255;
    if (tmp > 255) out.g = 255;
    else           out.g = tmp;
    tmp = c1.b * c2.b / 255;
    if (tmp > 255) out.b = 255;
    else           out.b = tmp;
    tmp = c1.a * c2.a / 255;
    if (tmp > 255) out.a = 255;
    else           out.a = tmp;
    return out;
}

#define c_white ns::Color(255,255,255)
#define c_black ns::Color(0,0,0)
#define c_red ns::Color(255,0,0)
#define c_green ns::Color(0,255,0)
#define c_blue ns::Color(0,0,255)
#define c_cyan ns::Color(0,255,255)
#define c_yellow ns::Color(255,255,0)
#define c_hgray ns::Color(128,128,128)

#define cf_white ns::Colorf(1,1,1)
#define cf_black ns::Colorf(0,0,0)
#define cf_red ns::Colorf(1,0,0)
#define cf_green ns::Colorf(0,1,0)
#define cf_blue ns::Colorf(0,0,1)
#define cf_cyan ns::Colorf(0,1,1)
#define cf_yellow ns::Colorf(1,1,0)
#define cf_hgray ns::Colorf(0.5,0.5,0.5)

} // namespace ns

#endif // COLOR_HEADER_INCLUDED
