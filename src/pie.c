#include "common.h"

#define PIE_FLAG_PALETTE      1
#define PIE_FLAG_TRANSPARENCY 2

typedef struct {
    u8 version;
    u8 flags;
    size width;
    size height;
    size length;
    u8 *data;
    u8 *palette;
} pie_image;

/* v2:
magic: 3
version: 1
flags: 1
width: 2 (LE)
height: 2 (LE)
length: 2 (LE)
data: ...
palette?: ...
*/

void pie_dimensions(byte *bytes, size *width, size *height) {
    *width = (size)*(u16 *)(&bytes[5]);
    *height = (size)*(u16 *)(&bytes[7]);
}

pie_image pie_from_bytes(byte *bytes) {
    pie_image r = {0};
    pie_dimensions(bytes, &r.width, &r.height);
    r.version = (u8)bytes[3];
    r.flags = (u8)bytes[4];
    r.length = (size)*(u16 *)(&bytes[9]);
    r.data = (u8 *)(&bytes[11]);

    if ((r.flags & PIE_FLAG_PALETTE) > 0) {
        r.palette = r.data + r.length * 2; // * 2 as it's length + index pairs. 
    }

    return r;
}

result pie_rgb_from_pie(pie_image p, allocator a) {
    u8rgb *pixels = make(u8rgb, p.width * p.height, a);
    size next_pixel_index = 0;
    // Assume RGB.
    u8 step = 3;

    // Must be RGBA.
    if ((p.flags & PIE_FLAG_TRANSPARENCY) > 0) {
        step = 4;
    }

    if ((p.flags & PIE_FLAG_PALETTE) > 0) {
        for (size i = 0; i < p.length; i += 1) {
            u8 run_length = p.data[i * 2];
            u8 color_index = p.data[i * 2 + 1] * step;

            for (u8 j = 0; j < run_length; j += 1) {
                size pixel_index = next_pixel_index++;
                pixels[pixel_index].r = p.palette[color_index + 0];
                pixels[pixel_index].g = p.palette[color_index + 1];
                pixels[pixel_index].b = p.palette[color_index + 2];
            }
        }
    }

    return (result){next_pixel_index, pixels};
}
