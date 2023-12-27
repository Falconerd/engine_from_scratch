#pragma once

#include "common.h"

#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82

//u32 *(*glGetString)(u32);
//void (*glClearColor)(f32, f32, f32, f32);
//void (*glClear)(u32);
// u32  (*glCreateShader)(u32);
void (*glShaderSource)(u32, size, char **, size *);
// void (*glCompileShader)(u32);
void (*glGetShaderiv)(u32, u32, i32 *);
void (*glGetShaderInfoLog)(u32, size, size *, char *);
u32  (*glCreateProgram)(void);
void (*glAttachShader)(u32 program, u32 shader);
void (*glLinkProgram)(u32 program);
void (*glGetProgramInfoLog)(u32 program, size maxlength, size *out_length, char *out_log);
void (*glGetProgramiv)(u32 program, u32 pname, i32 *params);

// OpenGL 1.x functions are supplied by Opengl32.lib.
u32 * glGetString(u32);
void glClearColor(f32, f32, f32, f32);
void glClear(u32);

// OpenGL 3.1 functions (our target) that need to be loaded on start.
TDFP(u32, glCreateShader, (u32));
TDFP(void, glCompileShader, (u32));
TDFP(void, glShaderSource, (u32, size, char **, size *));
TDFP(void, glGetShaderiv, (u32, u32, i32 *));
TDFP(void, glGetShaderInfoLog, (u32, size, size *, char *));
TDFP(u32, glCreateProgram, (void));
TDFP(void, glAttachShader, (u32 program, u32 shader));
TDFP(void, glLinkProgram, (u32 program));
TDFP(void, glGetProgramInfoLog, (u32 program, size maxlength, size *out_length, char *out_log));
TDFP(void, glGetProgramiv, (u32 program, u32 pname, i32 *params));
