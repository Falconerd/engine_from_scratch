#include "common.h"
#include "win32.h"

typedef struct {
    int half_transitions;
    b32 down;
} key_state;

typedef struct {
    key_state forward;
    key_state backward;
    key_state left;
    key_state right;
} input_state;

typedef struct game_state {
    b32 is_initialized;
    b32 is_running;
    u64 frame;
} game_state;

void game_update_and_render(game_memory *memory, input_state *input) {

    assert(sizeof(game_state) <= memory->permanent_storage_size);
    game_state *state = memory->permanent_storage;

    ////////////////////////////////////////////////////////////////////////////////////////////////

    f32 a = turn_y(0.f);    // expect 1
    f32 b = turn_y(0.125f); // expect ~0.7071
    f32 c = turn_y(0.25f);  // expect 0
    f32 d = turn_y(0.375f); // expect ~-0.7071
    f32 e = turn_y(0.5f);   // expect -1
    f32 f = turn_y(0.625f); // expect ~-0.7071
    f32 g = turn_y(0.75f);  // expect 0
    f32 h = turn_y(0.875f); // expect ~0.7071
    f32 i = turn_y(1.f);    // expect 1
    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    (void)f;
    (void)g;
    (void)h;
    (void)i;

    if (input->forward.down) {
        MessageBox(0, "ARST", "ARST", 0);
        input->forward.down = 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////

    state->frame += 1;
}
