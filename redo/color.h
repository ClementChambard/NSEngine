#ifndef COLOR_H
#define COLOR_H

struct Color {
    union {
        struct {
            float r;
            float g;
            float b;
            float a;
        };
        float components[4] = { 0, 0, 0, 1.0 };
    };

    unsigned int to_rgba32() const;
    unsigned int to_argb32() const;
    unsigned int to_abgr32() const;
    float get_h() const;
    float get_s() const;
    float get_v() const;
    void set_hsv(float hh, float ss, float vv, float aa = 1.0);

    inline float &operator[](int index) { return components[index]; }
    inline const float &operator[](int index) const { return components[index]; }

    bool operator==(const Color &col) const { return (r==col.r && g==col.g && b==col.b && a==col.a); }
    bool operator!=(const Color &col) const { return (r!=col.r || g!=col.g || b!=col.b || a!=col.a); }

    Color operator+(const Color &col) const;
    void operator+=(const Color &col);
    Color operator-() const;
    Color operator-(const Color &col) const;
    void operator-=(const Color &col);
    Color operator*(const Color &col) const;
    Color operator*(float scalar) const;
    void operator*=(const Color &col);
    void operator*=(float scalar);
    Color operator/(const Color &col) const;
    Color operator/(float scalar) const;
    void operator/=(const Color &col);
    void operator/=(float scalar);

    void invert();
    Color inverted() const;

    inline Color lefp(const Color &col_to, float weight) const {
        Color res = *this;

        res.r += (weight * (col_to.r - r));
        res.g += (weight * (col_to.g - g));
        res.b += (weight * (col_to.b - b));
        res.a += (weight * (col_to.a - a));

        return res;
    }

    inline Color darkened(float amount) const {
        Color res = *this;

        res.r = res.r * (1.0f - amount);
        res.g = res.g * (1.0f - amount);
        res.b = res.b * (1.0f - amount);

        return res;
    }

    inline Color lightened(float amount) const {
        Color res = *this;

        res.r = res.r + (1.0f - res.r) * amount;
        res.g = res.g + (1.0f - res.g) * amount;
        res.b = res.b + (1.0f - res.b) * amount;

        return res;
    }

    inline Color blend(const Color &over) const {
        Color res;
        float sa = 1.f - over.a;
        res.a = a * sa + over.a;
        if (res.a == 0) return Color{0, 0, 0, 0};
        else {
            res.r = (r * a * sa + over.r * over.a) / res.a;
            res.g = (g * a * sa + over.g * over.a) / res.a;
            res.b = (b * a * sa + over.b * over.a) / res.a;
        }
        return res;
    }

    static Color from_int(int hex_num);

    inline void set_r(unsigned char rr) { r = rr/255.0f; }
    inline void set_g(unsigned char gg) { g = gg/255.0f; }
    inline void set_b(unsigned char bb) { b = bb/255.0f; }
    inline void set_a(unsigned char aa) { a = aa/255.0f; }
    inline uint8_t get_r() const { return (uint8_t) (r * 255.0f); }
    inline uint8_t get_g() const { return (uint8_t) (g * 255.0f); }
    inline uint8_t get_b() const { return (uint8_t) (b * 255.0f); }
    inline uint8_t get_a() const { return (uint8_t) (a * 255.0f); }
    inline void set_h(float hh) { set_hsv(hh,get_s(),get_v()); }
    inline void set_s(float ss) { set_hsv(get_h(),ss,get_v()); }
    inline void set_v(float vv) { set_hsv(get_h(),get_s(),vv); }

};



#endif
