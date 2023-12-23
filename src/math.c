#include "common.h"

#define CONST_PI  3.141592f
#define CONST_TAU 6.283185f
#define TAYLOR_COUNT 6

// Dumb Cosine to learn Taylor Series. I still don't get it.
// NOTE: Right now I'm just converting the turn into radians by
// multiplying by TAU.
// TODO: Remove PI & TAU.
// TODO: Add SIMD.
// TODO: Investigate CPU hardware sin/cos.
f32 turn_x(f32 v) {
    v *= CONST_TAU;
    s32 div = (s32)(v / CONST_PI);
    v = v - (div * CONST_PI);
    s32 sign = 1;
    if (div % 2 != 0) {
        sign = -1;
    }

    f32 r = 1.f;
    f32 inter = 1.f;
    f32 num = v * v;

    for (int i = 1; i <= TAYLOR_COUNT; i += 1) {
        f32 comp = 2.f * i;
        f32 den = comp * (comp - 1.f);
        inter *= num / den;
        r += (inter - 2.f * (i & 1) * inter);
    }

    return (sign * r);
}

f32 turn_y(f32 v) {
    v += 0.25f;
    return turn_x(v);
}
