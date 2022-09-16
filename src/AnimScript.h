#ifndef ANIMSCRIPT
#define ANIMSCRIPT

#include "math/math.h"
#include "Interpolator.h"
#include "glm/glm.hpp"
#include "GameSprites.h"

#define ANM_FLAG_ACTIVE     0b0000000000000001
#define ANM_FLAG_VISIBLE    0b0000000000000010
#define ANM_FLAG_FLIP_X     0b0000000000000100
#define ANM_FLAG_FLIP_Y     0b0000000000001000
#define ANM_FLAG_DISABLE_3D 0b0000000000010000
#define ANM_FLAG_5          0b0000000000100000
#define ANM_FLAG_COLCHILD   0b0000000001000000
#define ANM_FLAG_GUI        0b0000000010000000
#define ANM_FLAG_8          0b0000000100000000
#define ANM_FLAG_9          0b0000001000000000
#define ANM_FLAG_10         0b0000010000000000
#define ANM_FLAG_11         0b0000100000000000
#define ANM_FLAG_OFFT       0b0001000000000000
#define ANM_FLAG_OFFB       0b0010000000000000
#define ANM_FLAG_OFFR       0b0100000000000000
#define ANM_FLAG_OFFL       0b1000000000000000

#define ituint8 NSEngine::NSITPuint8_t

class AnimScript;
typedef void (*animscript_t)(AnimScript*);

class AnimScript {
    public:
        AnimScript();
        void Init(animscript_t func_update);
        void SetScript(animscript_t func_update);
        void destroy();
        uint16_t create_child(animscript_t func_update, bool firstupdate = false);
        void Update(float framespeed); 
        void Draw();

        void set_parent_data(float x, float y, float z, float rx, float ry, float rz, float sx, float sy);

        void IsGui(bool gui);

        void special_draw(uint8_t i);

        void offset_sprite(int16_t o);
        void set_sprite(uint16_t s);                                                 //300
        void set_sprite_rand(uint16_t s, uint16_t b);                                //301
        //type                                                                       //302
        void set_blendmode(uint8_t i);                                               //303
        void set_layer(uint8_t l);                                                   //304
        void disable3d(bool i = true);                                               //305
        //void ins_306(bool enable);                                                 //306
        //void rand_mode(uint8_t mode);                                              //307
        void flip_x();                                                               //308
        void flip_y();                                                               //309
        void visible(bool visible);                                                  //310
        //void resample_mode(uint8_t n);                                             //311
        //void scroll_mode(uint8_t x, uint8_t y);                                    //312
        //void resolution_mode(uint8_t n);                                           //313
        //void ins_314(bool enable);                                                 //314
        void colorizeChildren(bool i = true);                                        //315
        //void ins_316();                                                            //316
        //void ins_317();                                                            //317
        void set_pos(float x, float y, float z);                                     //400
        void set_rot(float x, float y, float z);                                     //401
        void set_scale(float x, float y);                                            //402
        void set_alpha(uint8_t a);                                                   //403
        void set_color(uint8_t r, uint8_t g, uint8_t b);                             //404
        void set_alpha2(uint8_t a);                                                  //405
        void set_color2(uint8_t r, uint8_t g, uint8_t b);                            //406
        void pos_time(uint16_t t, uint8_t mode, float x, float y, float z);          //407
        void color_time(uint16_t t, uint8_t mode, uint8_t r, uint8_t g, uint8_t b);  //408
        void alpha_time(uint16_t t, uint8_t mode, uint8_t a);                        //409
        void rot_time(uint16_t t, uint8_t mode, float x, float y, float z);          //410
        //void rot_time2D(uint16_t t, uint8_t mode, float rz);                       //411
        void scale_time(uint16_t t, uint8_t mode, float x, float y);                 //412
        void color2_time(uint16_t t, uint8_t mode, uint8_t r, uint8_t g, uint8_t b); //413
        void alpha2_time(uint16_t t, uint8_t mode, uint8_t a);                       //414
        void rot_vel(float x, float y, float z);                                     //415
        void scale_vel(float x, float y);                                            //416
        //void alpha_time_linear(uint8_t a, uint16_t t);                             //417
        //void ins_418();                                                            //418
        //void ins_419(bool enable);                                                 //419
        void move_bezier(uint16_t t, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3); //420
        void anchor(uint8_t h, uint8_t v);                                           //421
        //void ins_422();                                                            //422
        //void color_mode(uint8_t mode);                                             //423
        //void rotate_auto(uint8_t mode);                                            //424
        void scroll_x(float amt);                                                    //425
        void scroll_y(float amt);                                                    //426
        void scroll_x_time(uint16_t t, uint8_t mode, float amt);                     //427
        void scroll_y_time(uint16_t t, uint8_t mode, float amt);                     //428
        void zoom(float u, float v);                                                 //429
        void zoom_time(uint16_t t, uint8_t mode, float u, float v);                  //430
        //void ins_431(bool enable);                                                 //431
        //void ins_432(bool enable);                                                 //432
        //void ins_433(uint16_t t, uint8_t mode, float x, float y);                  //433
        //void set_scale2(float x, float y);                                         //434
        //void scale2_time(uint16_t t, uint8_t mode, float x, float y);              //435
        void anchor_offset(float x, float y);                                        //436
        //void rotation_mode(uint8_t mode);                                          //437
        //void origin_mode(uint8_t mode);                                            //438
        //void fade_near_camera(bool enable, float far, float near);                 //439

        void raise_interrupt(uint8_t i);

        void x_time(uint16_t t, uint8_t mode, float x);
        void y_time(uint16_t t, uint8_t mode, float y);
        void z_time(uint16_t t, uint8_t mode, float z);
        float x_(bool p=false);
        float y_(bool p=false);
        float z_(bool p=false);
        float rx_(bool p=false) { return rot.x; }
        float ry_(bool p=false) { return rot.y; }
        float rz_(bool p=false) { return rot.z; }
        uint8_t cr_() { return col1.r; }
        uint8_t cg_() { return col1.g; }
        uint8_t cb_() { return col1.b; }
        uint8_t ca_() { return a; }
        void incx(float x);
        void incy(float y);
        void incz(float z);
        float xs_();
        float ys_();
        int spr_();

        bool isNotDrawable();

        uint16_t frame = 0;
        uint8_t interrupt = 0;

        float storage = 0;

    private:
        int16_t flags = 0b0000000000000010;
        uint16_t sprit = 0;
        uint8_t lyr = 0;
        uint8_t specialdraw = 0;
        uint8_t blendmode = 0;
        bool has_parent = 0;
        std::vector<uint16_t> childs_id;
        glm::vec3 pos   = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 rot   = glm::vec3(0.f, 0.f, 0.f);
        glm::vec2 scale = glm::vec2(1.f, 1.f);
        glm::vec2 uvz   = glm::vec2(1.f, 1.f);
        NSEngine::ColorRGB col1 = NSEngine::ColorRGB(255, 255, 255);
        NSEngine::ColorRGB col2 = NSEngine::ColorRGB(255, 255, 255);
        float u = 0.f, v = 0.f;
        ituint8 a = 255, a2 = 255;
        float rx_vel = 0.f, ry_vel = 0.f, rz_vel = 0.f;
        float sx_vel = 0.f, sy_vel = 0.f;
        float u_vel  = 0.f, v_vel  = 0.f;
        float su_vel = 0.f, sv_vel = 0.f;
        float px = 0.f, py = 0.f, pz=0.f;
        float prx = 0.f, pry = 0.f, prz = 0.f;
        float psx = 0.f, psy = 0.f;
        float ox = 0.f, oy = 0.f;
        animscript_t func_update;

        bool flag(uint16_t f);
        void flag(uint16_t f, bool set);

    friend class AnmManager;
    friend class AnimRenderer;
};

#endif
