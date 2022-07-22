#ifndef ASSETS_H
#define ASSETS_H

int defaultTexture();
int defaultFont();

class gameassets {
    public:
        static void Init();
        static int tex_ascii;
        static int se_alert;
        static int se_bonus2;
        static int se_bonus;
        static int se_boon00;
        static int se_boon01;
        static int se_cancel00;
        static int se_cardget;
        static int se_cat00;
        static int se_cat01;
        static int se_ch00;
        static int se_ch01;
        static int se_ch02;
        static int se_damage00;
        static int se_damage01;
        static int se_enep00;
        static int se_enep01;
        static int se_enep02;
        static int se_extend;
        static int se_graze;
        static int se_gun00;
        static int se_hint00;
        static int se_invalid;
        static int se_item00;
        static int se_kira00;
        static int se_kira01;
        static int se_kira02;
        static int se_lazer00;
        static int se_lazer01;
        static int se_msl;
        static int se_nep00;
        static int se_ok00;
        static int se_option;
        static int se_pause;
        static int se_pldead00;
        static int se_plst00;
        static int se_power0;
        static int se_power1;
        static int se_powerup;
        static int se_select00;
        static int se_slash;
        static int se_tan00;
        static int se_tan01;
        static int se_tan02;
        static int se_timeout2;
        static int se_timeout;
        static int se_warpl;
        static int se_warpr;
        static int se_water;
        static int fnt_base;
        static int fnt_small;
        static int fnt_score;
        static int fnt_bignumbers;
        static int fnt_score2;
        static int fnt_pickup1;
        static int fnt_pickup2;
        static int fnt_pickup3;
        static int mus_st01;
        static int mus_st01bs;
};

#endif