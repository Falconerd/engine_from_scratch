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

    ASSERT(sizeof(game_state) <= memory->permanent_storage_size);

    game_state *state = memory->permanent_storage;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    if (input->forward.down) {
        MessageBox(0, "ARST", "ARST", 0);
        input->forward.down = 0;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    state->frame += 1;
}

