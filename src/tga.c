
// typedef struct {
//    char  idlength;
//    char  colourmaptype;
//    char  datatypecode;
//    short int colourmaporigin;
//    short int colourmaplength;
//    char  colourmapdepth;
//    short int x_origin;
//    short int y_origin;
//    short width;
//    short height;
//    char  bitsperpixel;
//    char  imagedescriptor;
// } HEADER;

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} u8rgb;

void tga_dimensions(byte *tga_bytes, size *width, size *height) {
    byte *width_pos = &tga_bytes[12];
    byte *height_pos = &tga_bytes[14];
    *width = (size)(*(u16 *)width_pos);
    *height = (size)(*(u16 *)height_pos);
}

result rgb_from_tga(byte *tga_bytes, allocator a) {
    byte bits_per_pixel = tga_bytes[16];
    assert(bits_per_pixel == 32 && "Must be uncompressed 24-bit TGA with transparency.");
    size width, height;
    tga_dimensions(tga_bytes, &width, &height);
    size s = width * height;
    u8rgb *data = make(u8rgb, s, a);
    // TGA order is BGRA.
    // Kind of dumb to reorder it, but anyway.
    byte *bgr = &tga_bytes[18];
    for (size i = 0; i < width * height; i += 1) {
        data[i].r = bgr[i * 4 + 2];
        data[i].g = bgr[i * 4 + 1];
        data[i].b = bgr[i * 4 + 0];
    }
    return (result){s, data};
}
