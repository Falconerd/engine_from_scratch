typedef struct camera {
    v3 position;
    v3 front;
    v3 up;
    v3 right;
    v3 world_up;
    f32 yaw;
    f32 pitch;
    f32 movement_speed;
    f32 mouse_sensitivity;
    f32 zoom;
} camera;

typedef enum camera_movement {
    CAMERA_MOVEMENT_FORWARD,
    CAMERA_MOVEMENT_BACKWARD,
    CAMERA_MOVEMENT_LEFT,
    CAMERA_MOVEMENT_RIGHT,
} camera_movement;

const f32 CAMERA_YAW            = -90.f;
const f32 CAMERA_PITCH          = 0.f;
const f32 CAMERA_MOVEMENT_SPEED = 7.317f;
const f32 CAMERA_FLYING_SPEED   = 30;
const f32 CAMERA_BOOSTED_SPEED  = 80;
const f32 CAMERA_SENSITIVITY    = 0.1f;
const f32 CAMERA_ZOOM           = 45.f;
const v3  CAMERA_WORLD_UP       = {0, 1, 0};

void camera_update(camera *c) {
    c->front.x = mcosf(radians(c->yaw)) * mcosf(radians(c->pitch));
    c->front.y = msinf(radians(c->pitch));
    c->front.z = msinf(radians(c->yaw)) * mcosf(radians(c->pitch));
    c->front = v3_normalize(c->front);
    c->right = v3_normalize(v3_cross(c->front, c->world_up));
    c->up = v3_normalize(v3_cross(c->right, c->front));
}

void camera_init(camera *c, v3 p, v3 up, f32 yaw, f32 pitch) {
    c->position = p;
    c->world_up = up;
    c->yaw = yaw;
    c->pitch = pitch;

    c->front = v3(0, 0, -1.f);
    c->movement_speed = CAMERA_MOVEMENT_SPEED;
    c->mouse_sensitivity = CAMERA_SENSITIVITY;
    c->zoom = CAMERA_ZOOM;

    camera_update(c);
}

void camera_look_at(camera *c, v3 p) {
    v3 direction = v3_normalize(v3_sub(p, c->position));
    c->pitch = degrees(masinf(direction.y));
    if (direction.x == 0 && direction.z == 0) {
        c->yaw = 0;
    } else {
        c->yaw = degrees(matan2f(direction.z, direction.x));
    }

    if (c->yaw < -90.f) {
        c->yaw += 360.f;
    }

    camera_update(c);
}

void camera_view_matrix(camera *c, m4 *m) {
    m4_look_at(m, c->position, v3_add(c->position, c->front), c->up);
}

void camera_move_flying(camera *c, camera_movement d, f32 dt, b32 is_boosted) {
    f32 s = is_boosted ? CAMERA_BOOSTED_SPEED : CAMERA_FLYING_SPEED;
    f32 v = s * dt;
    v3 direction = {0};
    switch (d) {
        case CAMERA_MOVEMENT_FORWARD: {
            direction = v3_add(direction, c->front);
        } break;
        default:
            return;
    }

    if (v3_length(direction) > 0) {
        direction = v3_normalize(direction);
        c->position = v3_add(c->position, v3_scale(direction, v));
    }
}

void camera_move(camera *c, camera_movement d, f32 dt) {
    f32 v = c->movement_speed * dt;
    v3 direction = {0};
    switch (d) {
        case CAMERA_MOVEMENT_FORWARD: {
            direction = v3_add(direction, v3(c->front.x, 0, c->front.z));
        } break;
        case CAMERA_MOVEMENT_BACKWARD: {
            direction = v3_sub(direction, v3(c->front.x, 0, c->front.z));
        } break;
        case CAMERA_MOVEMENT_LEFT: {
            direction = v3_sub(direction, c->right);
        } break;
        case CAMERA_MOVEMENT_RIGHT: {
            direction = v3_add(direction, c->right);
        } break;
    }

    if (v3_length(direction) > 0) {
        direction = v3_normalize(direction);
        c->position = v3_add(c->position, v3_scale(direction, v));
    }
}

void camera_move_mouse(camera *c, f32 x, f32 y) {
    c->yaw += x * c->mouse_sensitivity;
    c->pitch -= y * c->mouse_sensitivity;

    if (c->pitch > 89.f) {
        c->pitch = 89.f;
    }
    if (c->pitch < -89.f) {
        c->pitch = -89.f;
    }

    camera_update(c);
}

