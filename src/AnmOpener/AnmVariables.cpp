#include "AnmVM.h"
#include "../NSEngine.h"
#include "../math/Random.h"

namespace NSEngine {

    int& AnmVM::check_ref(int i)
    {
        if      (i >= 10000 && i < 10004) return int_vars[static_cast<size_t>(i-10000)];
        else if (i == 10008) return script_var_8;
        else if (i == 10009) return script_var_9;
        //else if (i == 10029) return 65535;
        else return num_cycles_in_texture;
    }

    float& AnmVM::check_ref(float f)
    {
        if      (f >= 10004 && f < 10008) return float_vars[static_cast<size_t>(f-10004)];
        else if (f == 10013.f) return pos.current.x;
        else if (f == 10014.f) return pos.current.y;
        else if (f == 10015.f) return pos.current.z;
        else if (f == 10023.f) return rotation.current.x;
        else if (f == 10024.f) return rotation.current.y;
        else if (f == 10025.f) return rotation.current.z;
        else if (f == 10027.f) return rand_scale_1f;
        else if (f == 10028.f) return rand_scale_pi;
        else if (f == 10033.f) return rot_vars.x;
        else if (f == 10034.f) return rot_vars.y;
        else if (f == 10035.f) return rot_vars.z;
        else return float_vars[3];
    }

    int AnmVM::check_val(int i)
    {
        if      (i >= 10000 && i < 10004) return int_vars[static_cast<size_t>(i-10000)];
        else if (i == 10008) return script_var_8;
        else if (i == 10009) return script_var_9;
        else if (i == 10022) return static_cast<int>(Random::Float01() * 65535);
        else if (i == 10029) return 65535;
        else return i;
    }

    float AnmVM::check_val(float f)
    {
        if      (f >= 10004.f && f <  10008.f) return float_vars[static_cast<size_t>(f-10004.f)];
        else if (f == 10010.f || f == 10030.f) return Random::Floatm11() * rand_scale_pi;
        else if (f == 10011.f || f == 10031.f) return Random::Float01() * rand_scale_1f;
        else if (f == 10012.f || f == 10032.f) return Random::Floatm11() * rand_scale_1f;
        else if (f == 10013.f) return pos.current.x;
        else if (f == 10014.f) return pos.current.y;
        else if (f == 10015.f) return pos.current.z;
        else if (f == 10016.f) return (activeCamera3D() ? activeCamera3D()->getPosition().x : 0.f);
        else if (f == 10017.f) return (activeCamera3D() ? activeCamera3D()->getPosition().y : 0.f);
        else if (f == 10018.f) return (activeCamera3D() ? activeCamera3D()->getPosition().z : 0.f);
        else if (f == 10019.f) return (activeCamera3D() ? activeCamera3D()->getLook().x : 0.f);
        else if (f == 10020.f) return (activeCamera3D() ? activeCamera3D()->getLook().y : 0.f);
        else if (f == 10021.f) return (activeCamera3D() ? activeCamera3D()->getLook().z : 1.f);
        else if (f == 10023.f) return rotation.current.x;
        else if (f == 10024.f) return rotation.current.y;
        else if (f == 10025.f) return rotation.current.z;
        else if (f == 10026.f) return rotation.current.z + (parent ? parent->rotation.current.z : 0);
        else if (f == 10027.f) return rand_scale_1f;
        else if (f == 10028.f) return rand_scale_pi;
        else if (f == 10033.f) return rot_vars.x;
        else if (f == 10034.f) return rot_vars.y;
        else if (f == 10035.f) return rot_vars.z;
        else return f;
    }

}
