#include <stdarg.h>

#include "gfx.h"

namespace nx::gfx {

void printf(int x, int y, const char* fmt, ...)
{
    char buffer[1024];

    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, 1024, fmt, args);
    va_end(args);

    // push();
    begin();

    set_vertex_buffer(sprite_vb);
    set_indice_buffer(sprite_ib);
    set_blend_func(blend::src_alpha, blend::one_minus_src_alpha);
    set_render_state(BLEND | WRITE_RGBA);
    set_program(font_program);

    mat4x4 model, view, projection;

    int window_width, window_height;
    window_size(&window_width, &window_height);

    // NX_LOG_D("window_width %d", window_width);
    // NX_LOG_D("window_height %d", window_height);

    float eye[] = { 0, 0, 1 };
    float center[] = { 0, 0, 0 };
    float up[] = { 0, 1, 0 };
    mat4x4_look_at(view, eye, center, up);
    set_view_transform(view);

    mat4x4_ortho(projection, -window_width / 2, window_width / 2,
        -window_height / 2, window_height / 2, 0.1, 50.0);
    set_projection_transform(projection);

    char* ptr = buffer;
    while (*ptr) {
        auto ci = font_char_info(default_font, *ptr);
        set_texture(0, ci.texture);

        mat4x4_identity(model);
        mat4x4_scale_aniso(model, model, window_height, window_height, 1);

        set_model_transform(model);

        draw_element(0, 0, 6);
        ++ptr;
    }

    unused(view, projection);

    end();

    unused(x, y);
}

} // namespace nx::gfx