inline u32 safe_truncate_u64(u64 v) {
    ASSERT(v <= MAX_U32);
    u32 r = (u32)v;
    return r;
}

typedef struct game_state {
    b32 is_initialized;
    b32 is_running;
    u64 frame;
} game_state;

void game_update_and_render(game_memory *memory) {

    ASSERT(sizeof(game_state) <= memory->permanent_storage_size);
  
    ///////////////////////////////////////////////////////////////////////////////////////////////

    game_state *state = memory->permanent_storage;

    state->frame += 1;
}