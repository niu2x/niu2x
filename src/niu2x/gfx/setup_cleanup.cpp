#include "gfx.h"

namespace nx::gfx {

font_t* default_font = nullptr;

vertex_buffer_t* sprite_vb = nullptr;
indice_buffer_t* sprite_ib = nullptr;

void setup()
{

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    font::font_system_setup();

    default_font = create_builtin_font(24);

    auto sprite_vl
        = vertex_layout(vertex_attr_type::position, vertex_attr_type::uv);
    float vertices_data[4][6] = {
        { -0.5, -0.5, 0, 0, 1, 0 },
        { 0.5, -0.5, 0, 1, 1, 0 },
        { 0.5, 0.5, 0, 1, 0, 0 },
        { -0.5, 0.5, 0, 0, 0, 0 },
    };
    sprite_vb = create_vertex_buffer(sprite_vl, 4, vertices_data);

    uint32_t indice_data[] = {
        0,
        1,
        2,
        0,
        2,
        3,
    };
    sprite_ib = create_indice_buffer(6, indice_data);
}

void cleanup()
{
    destroy(sprite_vb);
    destroy(sprite_ib);
    destroy(default_font);
    font::font_system_cleanup();
}

} // namespace nx::gfx