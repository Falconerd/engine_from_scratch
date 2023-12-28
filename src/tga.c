
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

void rgb_from_tga(byte *tga_bytes, allocator a) {
    byte bits_per_pixel = &tga_bytes[16];
    assert(bits_per_pixel == 24 && "Must be uncompressed 24-bit TGA.");
    byte *width_pos = &tga_bytes[12];
    byte *height_pos = &tga_bytes[14];
    u16 width = *(u16 *)width_pos;
    u16 height = *(u16 *)height_pos;
    byte *data = make(byte, width * height * 3, a);
    // TGA order is BGR.
    // Kind of dumb to reorder it, but anyway.

    
}
