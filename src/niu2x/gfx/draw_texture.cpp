#include "gfx.h"

namespace nx::gfx {

void draw_texture(layer_t layer, texture_t* texture)
{
    static gfx::render_state_t render_state = gfx::WRITE_RGBA | gfx::BLEND;

    begin();

    gfx::reset();

    set_render_state(render_state);
    set_blend_func(blend::src_alpha, blend::one_minus_src_alpha);

    set_texture(0, texture);
    set_vertex_buffer(sprite_vb);
    set_indice_buffer(sprite_ib);
    set_program(sprite_program);

    int w, h;
    window_size(&w, &h);

    mat4x4 model;
    mat4x4_identity(model);
    mat4x4_translate(model, 0.5, 0.5, 0);

    mat4x4 scale;
    mat4x4_scale_aniso(scale, w, h, 0);
    mat4x4_mul(model, model, scale);
    set_model_transform(model);

    mat4x4 view;
    vec3 eye = { 0, 0, 1 };
    vec3 center = { 0, 0, 0 };
    vec3 up = { 0, 1, 0 };
    mat4x4_look_at(view, eye, center, up);
    set_view_transform(view);

    mat4x4 projection;
    mat4x4_ortho(projection, 0, w, 0, h, 0.1, 244.0);
    set_projection_transform(projection);

    draw_element(layer, 0, 6);

    end();
}

} // namespace nx::gfx
