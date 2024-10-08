#ifndef TEXT_INCLUDED
#define TEXT_INCLUDED

/*
    Let's go over some usage code first to get a feel for how this should work.
    Of course, initially, I don't really need anything fancy - perhaps some function that will
    fill a vertex buffer that can be sent to the GPU based on text given.
    F.E.:
        verts = text_write(font, text, allocator);
*/

typedef struct {
    v3 pos;
    v2 uvs;
    v4 color;
} text_vertex;

typedef struct {
    f32 advance;
    f32 l, b, r, t;
    f32 x, y, z, w; // positions, not pos and size.
} glyph;

// This currently only handles ASCII.
// It also wastes space using 0-32 of the array on the stack zeroed.
// This doesn't use the advance of the font!
// For the position, we will use coordinates as if they have been scaled,
// rather than between 0 and 1.
result text_write(v3 pos, v2 scale, glyph *font, s8 text, v4 color, allocator al) {
    v3 v = v3(pos.x * 1.f / scale.x, pos.y * 1.f / scale.y, pos.z);
    text_vertex *verts = make(text_vertex, text.length * 6, al);
    f32 texture_width  = font[31].x;
    f32 texture_height = font[31].y;
    size vertex_count = 0;

    for (size i = 0; i < text.length; i += 1) {
        if (text.data[i] < 31 || text.data[i] > 126) continue;
        if (text.data[i] == ' ') {
            v.x += font['A'].z - font['A'].x;
            continue;
        }

        glyph g = font[text.data[i]];

        v2 sz = v2(g.z - g.x, g.w - g.y);

        f32 p = g.x / texture_width;
        f32 q = g.y / texture_height;
        f32 r = g.z / texture_width;
        f32 s = g.w / texture_height;
       
        text_vertex *va = &verts[i * 6 + 0];
        text_vertex *vb = &verts[i * 6 + 1];
        text_vertex *vc = &verts[i * 6 + 2];
        text_vertex *vd = &verts[i * 6 + 3];
        text_vertex *ve = &verts[i * 6 + 4];
        text_vertex *vf = &verts[i * 6 + 5];

        // Align y, g, etc.
        f32 y = RENDER_HEIGHT - v.y - sz.y * g.b;

        *va = (text_vertex){v3(v.x, y, v.z), v2(p, q), color};
        *vb = (text_vertex){v3(v.x + sz.x, y, v.z), v2(r, q), color};
        *vc = (text_vertex){v3(v.x + sz.x, y - sz.y, v.z), v2(r, s), color};
        *vd = (text_vertex){v3(v.x, y - sz.y, v.z), v2(p, s), color};
        *ve = *va;
        *vf = *vc;

        v.x += sz.x;

        vertex_count += 6;
    }
    return (result){vertex_count, verts};
}

// NOTE: In this first version, the texture width and height are stored at 31[0] and 31[1].
glyph monofonto_atlas_data[] = {
    [31] = {0.f, 0.f, 0.f, 0.f, 0.f, 388.f, 388.f, 0.f, 0.f},
[32] = {0.5,0,0,0,0,0,0,0,0},
[33] = {0.5f,0.140625f,-0.048750f,0.359375f,0.763750f,366.5f,335.5f,380.5f,387.5},
[34] = {0.5f,0.063f,0.428437f,0.438f,0.756562f,353.5f,35.5f,377.5f,56.5},
[35] = {0.5f,-0.015625f,-0.035937f,0.515625f,0.760937f,161.5f,212.5f,195.5f,263.5},
[36] = {0.5f,0.008312f,-0.137499f,0.4926875f,0.862500f,203.5f,323.5f,234.5f,387.5},
[37] = {0.5f,-0.045874f,-0.051562f,0.547875f,0.776562f,327.5f,334.5f,365.5f,387.5},
[38] = {0.5f,-0.025750f,-0.043750f,0.536750f,0.768750f,0.5f,264.5f,36.5f,316.5},
[39] = {0.5f,0.1645625f,0.428437f,0.3364375f,0.756562f,376.5f,295.5f,387.5f,316.5},
[40] = {0.5f,0.180375f,-0.237875f,0.461625f,0.855875f,0.5f,317.5f,18.5f,387.5},
[41] = {0.5f,0.038875f,-0.237875f,0.320124f,0.855875f,19.5f,317.5f,37.5f,387.5},
[42] = {0.5f,-0.030249f,0.213562f,0.53225f,0.760437f,200.5f,21.5f,236.5f,56.5},
[43] = {0.5f,-0.007812f,0.101187f,0.507812f,0.616812f,237.5f,23.5f,270.5f,56.5},
[44] = {0.5f,0.124125f,-0.170187f,0.342874f,0.189187f,338.5f,33.5f,352.5f,56.5},
[45] = {0.5f,0.062f,0.281562f,0.437f,0.453437f,71.5f,7.5f,95.5f,18.5},
[46] = {0.5f,0.140625f,-0.048187f,0.359375f,0.186187f,21.5f,3.5f,35.5f,18.5},
[47] = {0.5f,-0.023437f,-0.205625f,0.5234375f,0.825625f,131.5f,321.5f,166.5f,387.5},
[48] = {0.5f,0.0234375f,-0.043750f,0.4765625f,0.768750f,37.5f,264.5f,66.5f,316.5},
[49] = {0.5f,0.067062f,-0.038750f,0.363937f,0.773750f,67.5f,264.5f,86.5f,316.5},
[50] = {0.5f,0.009625f,-0.038750f,0.478374f,0.773750f,87.5f,264.5f,117.5f,316.5},
[51] = {0.5f,0.001999f,-0.043750f,0.502f,0.768750f,118.5f,264.5f,150.5f,316.5},
[52] = {0.5f,-0.015625f,-0.035937f,0.515625f,0.760937f,248.5f,212.5f,282.5f,263.5},
[53] = {0.5f,0.008812f,-0.048750f,0.4931875f,0.763750f,151.5f,264.5f,182.5f,316.5},
[54] = {0.5f,-1.7763560f, -0.048750f,0.5f,0.763750f,183.5f,264.5f,215.5f,316.5},
[55] = {0.5f,0.006000f,-0.035937f,0.506000f,0.760937f,129.5f,159.5f,161.5f,210.5},
[56] = {0.5f,-1.7763560f, -0.043750f,0.5f,0.768750f,216.5f,264.5f,248.5f,316.5},
[57] = {0.5f,-1.7763560f, -0.038750f,0.5f,0.773750f,249.5f,264.5f,281.5f,316.5},
[58] = {0.5f,0.140625f,0.068374f,0.359375f,0.599624f,372.5f,72.5f,386.5f,106.5},
[59] = {0.5f,0.123812f,-0.064937f,0.358187f,0.606937f,64.5f,63.5f,79.5f,106.5},
[60] = {0.5f,-0.008312f,0.055812f,0.507312f,0.665187f,114.5f,67.5f,147.5f,106.5},
[61] = {0.5f,-0.007812f,0.173000f,0.507812f,0.548000f,304.5f,32.5f,337.5f,56.5},
[62] = {0.5f,-0.008312f,0.055812f,0.507312f,0.665187f,80.5f,67.5f,113.5f,106.5},
[63] = {0.5f,0.011937f,-0.043750f,0.465062f,0.768750f,282.5f,264.5f,311.5f,316.5},
[64] = {0.5f,-0.015625f,-0.215999f,0.515625f,0.659000f,292.5f,331.5f,326.5f,387.5},
[65] = {0.5f,-0.015625f,-0.035937f,0.515625f,0.760937f,353.5f,212.5f,387.5f,263.5},
[66] = {0.5f,0.014312f,-0.035937f,0.498687f,0.760937f,248.5f,107.5f,279.5f,158.5},
[67] = {0.5f,0.004812f,-0.043750f,0.4891875f,0.768750f,312.5f,264.5f,343.5f,316.5},
[68] = {0.5f,0.018625f,-0.035937f,0.487375f,0.760937f,217.5f,107.5f,247.5f,158.5},
[69] = {0.5f,0.032250f,-0.035937f,0.46975f,0.760937f,188.5f,107.5f,216.5f,158.5},
[70] = {0.5f,0.017625f,-0.035937f,0.486375f,0.760937f,157.5f,107.5f,187.5f,158.5},
[71] = {0.5f,0.004812f,-0.043750f,0.4891875f,0.768750f,344.5f,264.5f,375.5f,316.5},
[72] = {0.5f,0.015625f,-0.035937f,0.484375f,0.760937f,89.5f,107.5f,119.5f,158.5},
[73] = {0.5f,0.046875f,-0.035937f,0.453125f,0.760937f,62.5f,107.5f,88.5f,158.5},
[74] = {0.5f,0.024625f,-0.035937f,0.493375f,0.760937f,31.5f,107.5f,61.5f,158.5},
[75] = {0.5f,0.013499f,-0.035937f,0.513499f,0.760937f,355.5f,159.5f,387.5f,210.5},
[76] = {0.5f,0.016125f,-0.035937f,0.484875f,0.760937f,0.5f,107.5f,30.5f,158.5},
[77] = {0.5f,-3.552713f,-0.035937f,0.5f,0.760937f,322.5f,159.5f,354.5f,210.5},
[78] = {0.5f,0.015625f,-0.035937f,0.484375f,0.760937f,291.5f,159.5f,321.5f,210.5},
[79] = {0.5f,0.007812f,-0.043750f,0.4921875f,0.768750f,64.5f,211.5f,95.5f,263.5},
[80] = {0.5f,0.017312f,-0.035937f,0.501687f,0.760937f,227.5f,159.5f,258.5f,210.5},
[81] = {0.5f,0.007812f,-0.1514375f,0.4921875f,0.7704375f,260.5f,328.5f,291.5f,387.5},
[82] = {0.5f,0.014999f,-0.035937f,0.515000f,0.760937f,162.5f,159.5f,194.5f,210.5},
[83] = {0.5f,0.008312f,-0.043750f,0.4926875f,0.768750f,96.5f,211.5f,127.5f,263.5},
[84] = {0.5f,-0.007812f,-0.035937f,0.507812f,0.760937f,95.5f,159.5f,128.5f,210.5},
[85] = {0.5f,-3.552713f,-0.048750f,0.5f,0.763750f,128.5f,211.5f,160.5f,263.5},
[86] = {0.5f,-0.015625f,-0.035937f,0.515625f,0.760937f,34.5f,159.5f,68.5f,210.5},
[87] = {0.5f,-0.030750f,-0.035937f,0.531750f,0.760937f,120.5f,107.5f,156.5f,158.5},
[88] = {0.5f,-0.007812f,-0.035937f,0.507812f,0.760937f,0.5f,159.5f,33.5f,210.5},
[89] = {0.5f,-0.022937f,-0.035937f,0.523937f,0.760937f,317.5f,212.5f,352.5f,263.5},
[90] = {0.5f,-0.011312f,-0.035937f,0.504312f,0.760937f,283.5f,212.5f,316.5f,263.5},
[91] = {0.5f,0.219187f,-0.237374f,0.484812f,0.856375f,61.5f,317.5f,78.5f,387.5},
[92] = {0.5f,-0.023437f,-0.205625f,0.5234375f,0.825625f,167.5f,321.5f,202.5f,387.5},
[93] = {0.5f,0.0161875f,-0.237374f,0.281812f,0.856375f,102.5f,317.5f,119.5f,387.5},
[94] = {0.5f,-7.105427f,0.3671875f,0.5f,0.7578125f,271.5f,31.5f,303.5f,56.5},
[95] = {0.5f,-0.03125f,-0.2303125f,0.53125f,-0.089687f,96.5f,9.5f,132.5f,18.5},
[96] = {0.5f,0.09325f,0.520687f,0.40575f,0.786312f,0.5f,1.5f,20.5f,18.5},
[97] = {0.5f,-0.005000f,-0.047374f,0.495f,0.546375f,181.5f,68.5f,213.5f,106.5},
[98] = {0.5f,0.016312f,-0.048750f,0.500687f,0.763750f,32.5f,211.5f,63.5f,263.5},
[99] = {0.5f,-0.002000f,-0.047374f,0.498f,0.546375f,276.5f,68.5f,308.5f,106.5},
[100] = {0.5f,-0.000687f,-0.048750f,0.483687f,0.763750f,0.5f,211.5f,31.5f,263.5},
[101] = {0.5f,0.001999f,-0.047374f,0.502f,0.546375f,148.5f,68.5f,180.5f,106.5},
[102] = {0.5f,0.012312f,-0.035937f,0.4966875f,0.760937f,259.5f,159.5f,290.5f,210.5},
[103] = {0.5f,-0.000187f,-0.240625f,0.484187f,0.540625f,344.5f,108.5f,375.5f,158.5},
[104] = {0.5f,0.013812f,-0.035937f,0.498187f,0.760937f,195.5f,159.5f,226.5f,210.5},
[105] = {0.5f,0.0546875f,-0.033937f,0.4453125f,0.762937f,69.5f,159.5f,94.5f,210.5},
[106] = {0.5f,0.048500f,-0.234999f,0.423499f,0.765000f,235.5f,323.5f,259.5f,387.5},
[107] = {0.5f,0.011312f,-0.035937f,0.4956875f,0.760937f,216.5f,212.5f,247.5f,263.5},
[108] = {0.5f,0.074062f,-0.035937f,0.370937f,0.760937f,196.5f,212.5f,215.5f,263.5},
[109] = {0.5f,-0.007812f,-0.034562f,0.5078125f,0.543562f,32.5f,19.5f,65.5f,56.5},
[110] = {0.5f,0.013812f,-0.034562f,0.498187f,0.543562f,0.5f,19.5f,31.5f,56.5},
[111] = {0.5f,-3.552713f,-0.047374f,0.5f,0.546375f,214.5f,68.5f,246.5f,106.5},
[112] = {0.5f,0.016312f,-0.235625f,0.500687f,0.545625f,280.5f,108.5f,311.5f,158.5},
[113] = {0.5f,-0.000687f,-0.235625f,0.483687f,0.545625f,312.5f,108.5f,343.5f,158.5},
[114] = {0.5f,0.022125f,-0.034562f,0.490875f,0.543562f,341.5f,69.5f,371.5f,106.5},
[115] = {0.5f,0.033750f,-0.047374f,0.47125f,0.546375f,247.5f,68.5f,275.5f,106.5},
[116] = {0.5f,0.014937f,-0.034062f,0.468062f,0.6690625f,34.5f,61.5f,63.5f,106.5},
[117] = {0.5f,0.007812f,-0.045062f,0.4921875f,0.533062f,309.5f,69.5f,340.5f,106.5},
[118] = {0.5f,-0.007812f,-0.032250f,0.5078125f,0.53025f,102.5f,20.5f,135.5f,56.5},
[119] = {0.5f,-0.022937f,-0.032250f,0.523937f,0.53025f,66.5f,20.5f,101.5f,56.5},
[120] = {0.5f,0.007812f,-0.032250f,0.4921875f,0.53025f,168.5f,20.5f,199.5f,56.5},
[121] = {0.5f,-0.007812f,-0.233312f,0.5078125f,0.532312f,0.5f,57.5f,33.5f,106.5},
[122] = {0.5f,0.003312f,-0.032250f,0.4876875f,0.53025f,136.5f,20.5f,167.5f,56.5},
[123] = {0.5f,0.141125f,-0.237374f,0.484875f,0.856375f,79.5f,317.5f,101.5f,387.5},
[124] = {0.5f,0.171875f,-0.205625f,0.328125f,0.825625f,120.5f,321.5f,130.5f,387.5},
[125] = {0.5f,0.016125f,-0.237374f,0.359875f,0.856375f,38.5f,317.5f,60.5f,387.5},
[126] = {0.5f,-0.015625f,0.251624f,0.515625f,0.470374f,36.5f,4.5f,70.5f,18.5}

};

#endif

