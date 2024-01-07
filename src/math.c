#ifndef MATH_INCLUDED
#define MATH_INCLUDED

#include "common.h"

#define CONST_PI  3.141592f
#define CONST_HALF_PI  1.570796f
#define CONST_TAU 6.283185f
#define TAYLOR_COUNT 6

// TOOD: Fill out these functions.
f32 msqrtf(f32 v) {
    f32 x = v * 0.5f;
    f32 y = v;
    i32 i = *(i32 *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(f32*)&i;
    y = y * (1.5f - (x * y * y));
    y = y * (1.5f - (x * y * y));
    return v * y;
};

f32 mtanf(f32 v) { (void)v; return 0.f; };
f32 msinf(f32 v) {
    (void)v; return 0;
    // i32 n = (x / CONST_HALF_PI + 0.5f);
    // v -= n * CONST_HALF_PI;
    // n = mod32(n, (i32)4);
    // switch (n) {
    //     case 0:
    //         return _sine(v);
    //     case 1:
    //         return _cosine(v);
    //     case 2:
    //         return -_sine(v);
    //     case 3:
    //         return -_cosine(v);
    // }
};

f32 mcosf(f32 v) {
    return msinf(v + CONST_HALF_PI);
};

u32 mrand(u32 v) {
    v = (v << 13) ^ v;
    return ((v * (v * v * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

// Dumb Cosine to learn Taylor Series. I still don't get it.
// NOTE: Right now I'm just converting the turn into radians by
// multiplying by TAU.
// TODO: Remove PI & TAU.
// TODO: Add SIMD.
// TODO: Investigate CPU hardware sin/cos.
f32 turn_x(f32 v) {
    v *= CONST_TAU;
    i32 div = (i32)(v / CONST_PI);
    v = v - (div * CONST_PI);
    i32 sign = 1;
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
    v -= 0.25f;
    return turn_x(v);
}

f32 degrees(f32 r) {
    return r * (180.f / CONST_PI);
}

f32 radians(f32 d) {
    return d * (CONST_PI / 180.f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

v3 v3_scale(v3 v, f32 s) {
    return v3(v.x * s, v.y * s, v.z * s);
}

v3 v3_normalize(v3 v) {
    f32 length = msqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return v3(v.x / length, v.y / length, v.z / length);
}

v3 v3_add(v3 a, v3 b) {
    return v3(a.x + b.x, a.y + b.y, a.z + b.z);
}

v3 v3_sub(v3 a, v3 b) {
    return v3(a.x - b.x, a.y - b.y, a.z - b.z);
}

f32 v3_length(v3 v) {
    return msqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

v3 v3_cross(v3 a, v3 b) {
    return v3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

f32 v3_dot(v3 a, v3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

m4 m4_identity(void) {
    return (m4){
        .data = {{1, 0, 0, 0},
                 {0, 1, 0, 0},
                 {0, 0, 1, 0},
                 {0, 0, 0, 1}}
    };
}

void m4_rotate(m4 *result, f32 angle, v3 axis) {
    v3 norm_axis = v3_normalize(axis);
    f32 x = norm_axis.x, y = norm_axis.y, z = norm_axis.z;
    f32 c = mcosf(angle), s = msinf(angle);
    f32 omc = 1.0f - c;

    result->data[0][0] = x * x * omc + c;
    result->data[0][1] = y * x * omc + z * s;
    result->data[0][2] = x * z * omc - y * s;
    result->data[0][3] = 0.0f;

    result->data[1][0] = x * y * omc - z * s;
    result->data[1][1] = y * y * omc + c;
    result->data[1][2] = y * z * omc + x * s;
    result->data[1][3] = 0.0f;

    result->data[2][0] = x * z * omc + y * s;
    result->data[2][1] = y * z * omc - x * s;
    result->data[2][2] = z * z * omc + c;
    result->data[2][3] = 0.0f;

    result->data[3][0] = 0.0f;
    result->data[3][1] = 0.0f;
    result->data[3][2] = 0.0f;
    result->data[3][3] = 1.0f;
}

void m4_translate(m4 *result, v3 translation) {
    result->data[0][0] = 1.0f;
    result->data[0][1] = 0.0f;
    result->data[0][2] = 0.0f;
    result->data[0][3] = translation.x;

    result->data[1][0] = 0.0f;
    result->data[1][1] = 1.0f;
    result->data[1][2] = 0.0f;
    result->data[1][3] = translation.y;

    result->data[2][0] = 0.0f;
    result->data[2][1] = 0.0f;
    result->data[2][2] = 1.0f;
    result->data[2][3] = translation.z;

    result->data[3][0] = 0.0f;
    result->data[3][1] = 0.0f;
    result->data[3][2] = 0.0f;
    result->data[3][3] = 1.0f;
}

void m4_ortho(m4 *m, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f) {
    m->data[0][0] = 2.f / (r - l);
    m->data[0][1] = m->data[0][2] = m->data[0][3] = 0.f;

    m->data[1][1] = 2.f / (t - b);
    m->data[1][0] = m->data[1][2] = m->data[1][3] = 0.f;

    m->data[2][2] = -2.f / (f - n);
    m->data[2][0] = m->data[2][1] = m->data[2][3] = 0.f;

    m->data[3][0] = -(r + l) / (r - l);
    m->data[3][1] = -(t + b) / (t - b);
    m->data[3][2] = -(f + n) / (f - n);
    m->data[3][3] = 1.f;
}

void m4_multiply(m4 *result, m4 *a, m4 *b) {
    m4 temp = {0};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp.data[i][j] = a->data[i][0] * b->data[0][j] +
                              a->data[i][1] * b->data[1][j] +
                              a->data[i][2] * b->data[2][j] +
                              a->data[i][3] * b->data[3][j];
        }
    }
    *result = temp;
}

void m4_scale(m4 *result, v3 scale) {
    m4 scaling_matrix = {
        .data = {
            {scale.x, 0, 0, 0},
            {0, scale.y, 0, 0},
            {0, 0, scale.z, 0},
            {0, 0, 0, 1}
        }
    };

    m4_multiply(result, result, &scaling_matrix);
}

void m4_perspective(m4 *m, f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane) {
    f32 tan_half_fov = mtanf(fov / 2.f);
    m->data[0][0] = 1.f / (aspect_ratio * tan_half_fov);
    m->data[1][1] = 1.f / tan_half_fov;
    m->data[2][2] = -(far_plane + near_plane) / (far_plane - near_plane);
    m->data[2][3] = -1.f;
    m->data[3][2] = -(2.f * far_plane * near_plane) / (far_plane - near_plane);

    m->data[0][1] = m->data[0][2] = m->data[0][3] = 0.f;
    m->data[1][0] = m->data[1][2] = m->data[1][3] = 0.f;
    m->data[2][0] = m->data[2][1] = 0.f;
    m->data[3][0] = m->data[3][1] = m->data[3][3] = 0.f;
}

void m4_look_at(m4 *result, v3 eye, v3 center, v3 up) {
    v3 f, s, u;
    f = v3_normalize(v3_sub(center, eye)); // Forward vector
    s = v3_normalize(v3_cross(f, up)); // Right vector
    u = v3_cross(s, f);

    result->data[0][0] = s.x;
    result->data[1][0] = s.y;
    result->data[2][0] = s.z;
    result->data[0][1] = u.x;
    result->data[1][1] = u.y;
    result->data[2][1] = u.z;
    result->data[0][2] = -f.x;
    result->data[1][2] = -f.y;
    result->data[2][2] = -f.z;
    result->data[3][0] = -v3_dot(s, eye);
    result->data[3][1] = -v3_dot(u, eye);
    result->data[3][2] = v3_dot(f, eye);
    result->data[0][3] = 0.0f;
    result->data[1][3] = 0.0f;
    result->data[2][3] = 0.0f;
    result->data[3][3] = 1.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif

