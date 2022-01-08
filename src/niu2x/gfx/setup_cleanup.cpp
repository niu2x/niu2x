#include "gfx.h"

namespace nx::gfx {

font_t* default_font = nullptr;
program_t* sprite_program = nullptr;
vertex_buffer_t* sprite_vb = nullptr;
indice_buffer_t* sprite_ib = nullptr;

static const char* sprite_program_source[] = { R"RAW(
#version 300 es

layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 uv;

uniform mat4 MVP;
out highp vec3 v_uv;

void main()
{
    gl_Position =  vec4(position, 1.0) * MVP;
    v_uv = uv;
}

)RAW",
    R"RAW(
#version 300 es

uniform sampler2D TEX0;
in highp vec3 v_uv;

out highp vec4 color;

void main()
{
    color = texture(TEX0, v_uv.xy);
}

)RAW" };

void setup()
{

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    font::font_system_setup();

    default_font = font_create(24);

    auto sprite_vl = vertex_layout(vertex_attr_t::position, vertex_attr_t::uv);
    float vertices_data[4][6] = {
        { -0.5, -0.5, 0, 0, 1, 0 },
        { 0.5, -0.5, 0, 1, 1, 0 },
        { 0.5, 0.5, 0, 1, 0, 0 },
        { -0.5, 0.5, 0, 0, 0, 0 },
    };
    sprite_vb = vertex_buffer_create(sprite_vl, 4, vertices_data);

    uint32_t indice_data[] = {
        0,
        1,
        2,
        0,
        2,
        3,
    };
    sprite_ib = indice_buffer_create(6, indice_data);

    sprite_program = program_create(sprite_program_source);

    render_setup();
}

void cleanup()
{
    render_cleanup();

    destroy(sprite_program);
    destroy(sprite_vb);
    destroy(sprite_ib);
    destroy(default_font);
    font::font_system_cleanup();
}

} // namespace nx::gfx