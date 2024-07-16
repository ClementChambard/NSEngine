#include "./Interp.hpp"
#include "math/math.hpp"

namespace ns {

f32 InterpFunc_00(f32 t) { return t; }
f32 InterpFunc_01(f32 t) { return t * t; }
f32 InterpFunc_02(f32 t) { return t * t * t; }
f32 InterpFunc_03(f32 t) { return t * t * t * t; }
f32 InterpFunc_04(f32 t) { t = 1 - t; return 1 - (t * t); }
f32 InterpFunc_05(f32 t) { t = 1 - t; return 1 - (t * t * t); }
f32 InterpFunc_06(f32 t) { t = 1 - t; return 1 - (t * t * t * t); }
f32 InterpFunc_07(f32 t) { return t; }
f32 InterpFunc_08(f32 t) { return 2 * t*t*t + 3 * t*t; }
f32 InterpFunc_09(f32 t) {
    if (t < 0.5) return 2 * t * t;
    else return 0.5f+0.5f*(InterpFunc_04(2*t-1));
}
f32 InterpFunc_10(f32 t) {
    if (t < 0.5) return 0.5f*(InterpFunc_02(2*t));
    else return 0.5f+0.5f*(InterpFunc_05(2*t-1));
}
f32 InterpFunc_11(f32 t) {
    if (t < 0.5) return 0.5f*(InterpFunc_03(2*t));
    else return 0.5f+0.5f*(InterpFunc_06(2*t-1));
}
f32 InterpFunc_12(f32 t) {
    if (t < 0.5) return 0.5f*(InterpFunc_04(2*t));
    else return 0.5f+0.5f*(InterpFunc_01(2*t-1));
}
f32 InterpFunc_13(f32 t) {
    if (t < 0.5) return 0.5f*(InterpFunc_05(2*t));
    else return 0.5f+0.5f*(InterpFunc_02(2*t-1));
}
f32 InterpFunc_14(f32 t) {
    if (t < 0.5) return 0.5f*(InterpFunc_06(2*t));
    else return 0.5f+0.5f*(InterpFunc_03(2*t-1));
}
f32 InterpFunc_15(f32 t) { return t >= 1 ? 1 : 0; }
f32 InterpFunc_16(f32 t) { return t > 0 ? 1 : 0; }
f32 InterpFunc_17(f32 t) { return t * t; }
f32 InterpFunc_18(f32 t) { return sin(t * ns::PI_1_2<f32>); }
f32 InterpFunc_19(f32 t) { return 1 - sin((1-t) * ns::PI_1_2<f32>); }
f32 InterpFunc_20(f32 t) {
    if (t < 0.5) return 0.5f*(InterpFunc_19(2*t));
    else return 0.5f+0.5f*(InterpFunc_18(2*t-1));
}
f32 InterpFunc_21(f32 t) {
    if (t < 0.5) return 0.5f*(InterpFunc_18(2*t));
    else return 0.5f+0.5f*(InterpFunc_19(2*t-1));
}
f32 InterpFunc_22(f32 t) {return (((t - 0.25) * (t - 0.25) / 0.5625) - 0.111111) / 0.888889;}
f32 InterpFunc_23(f32 t) {return (((t - 0.3 ) * (t - 0.3 ) / 0.49) - 0.183673) / 0.816326;}
f32 InterpFunc_24(f32 t) {return (((t - 0.35) * (t - 0.35) / 0.4225) - 0.289941) / 0.710059;}
f32 InterpFunc_25(f32 t) {return (((t - 0.38) * (t - 0.38) / 0.3844) - 0.37565) / 0.62435;}
f32 InterpFunc_26(f32 t) {return (((t - 0.4 ) * (t - 0.4 ) / 0.36) - 0.444444) / 0.555556;}
f32 InterpFunc_27(f32 t) { return 1 - InterpFunc_22(1 - t); }
f32 InterpFunc_28(f32 t) { return 1 - InterpFunc_23(1 - t); }
f32 InterpFunc_29(f32 t) { return 1 - InterpFunc_24(1 - t); }
f32 InterpFunc_30(f32 t) { return 1 - InterpFunc_25(1 - t); }
f32 InterpFunc_31(f32 t) { return 1 - InterpFunc_26(1 - t); }

PFN_InterpFunc getInterpFunc(InterpMethod mode) {
  static const PFN_InterpFunc funcs[] = {
    InterpFunc_00,
    InterpFunc_01,
    InterpFunc_02,
    InterpFunc_03,
    InterpFunc_04,
    InterpFunc_05,
    InterpFunc_06,
    InterpFunc_07,
    InterpFunc_08,
    InterpFunc_09,
    InterpFunc_10,
    InterpFunc_11,
    InterpFunc_12,
    InterpFunc_13,
    InterpFunc_14,
    InterpFunc_15,
    InterpFunc_16,
    InterpFunc_17,
    InterpFunc_18,
    InterpFunc_19,
    InterpFunc_20,
    InterpFunc_21,
    InterpFunc_22,
    InterpFunc_23,
    InterpFunc_24,
    InterpFunc_25,
    InterpFunc_26,
    InterpFunc_27,
    InterpFunc_28,
    InterpFunc_29,
    InterpFunc_30,
    InterpFunc_31,
  };
  u8 m = u8(mode);
  if (m > 31) return InterpFunc_00;
  return funcs[m];
}

}
