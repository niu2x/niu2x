#include <stdarg.h>

#include <vector>
#include <map>

#include "gfx.h"

namespace nx::gfx {

void printf(int x, int y, const char* fmt, ...)
{
    char buffer[1024];

    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, 1024, fmt, args);
    va_end(args);

    std::map<texture_t*, std::vector<float>> vdmap;

    int pen_x = 0;
    int pen_y = 0;

    char* ptr = buffer;

    size_t max_vertex_count = 0;

    while (*ptr) {
        auto ci = font_char_info(default_font, *ptr);

        if (vdmap.find(ci.texture) == vdmap.end()) {
            vdmap[ci.texture] = std::vector<float>();
        }

        vdmap[ci.texture].emplace_back(pen_x + ci.x);
        vdmap[ci.texture].emplace_back(pen_y + ci.y);
        vdmap[ci.texture].emplace_back(0);
        vdmap[ci.texture].emplace_back(ci.uv_x);
        vdmap[ci.texture].emplace_back(ci.uv_y + ci.uv_h);
        vdmap[ci.texture].emplace_back(0);

        vdmap[ci.texture].emplace_back(pen_x + ci.x + ci.w);
        vdmap[ci.texture].emplace_back(pen_y + ci.y);
        vdmap[ci.texture].emplace_back(0);
        vdmap[ci.texture].emplace_back(ci.uv_x + ci.uv_w);
        vdmap[ci.texture].emplace_back(ci.uv_y + ci.uv_h);
        vdmap[ci.texture].emplace_back(0);

        vdmap[ci.texture].emplace_back(pen_x + ci.x + ci.w);
        vdmap[ci.texture].emplace_back(pen_y + ci.y + ci.h);
        vdmap[ci.texture].emplace_back(0);
        vdmap[ci.texture].emplace_back(ci.uv_x + ci.uv_w);
        vdmap[ci.texture].emplace_back(ci.uv_y);
        vdmap[ci.texture].emplace_back(0);

        vdmap[ci.texture].emplace_back(pen_x + ci.x);
        vdmap[ci.texture].emplace_back(pen_y + ci.y + ci.h);
        vdmap[ci.texture].emplace_back(0);
        vdmap[ci.texture].emplace_back(ci.uv_x);
        vdmap[ci.texture].emplace_back(ci.uv_y);
        vdmap[ci.texture].emplace_back(0);

        max_vertex_count = std::max(max_vertex_count, vdmap[ci.texture].size());

        if (*(ptr + 1))
            pen_x += ci.advance + font_kerning(default_font, *ptr, *(ptr + 1));

        ++ptr;
    }

    max_vertex_count /= 6;

    std::vector<uint32_t> indice_data;
    for (size_t i = 0; i < (max_vertex_count >> 2); i++) {
        indice_data.emplace_back((i << 2) + 0);
        indice_data.emplace_back((i << 2) + 1);
        indice_data.emplace_back((i << 2) + 2);
        indice_data.emplace_back((i << 2) + 0);
        indice_data.emplace_back((i << 2) + 2);
        indice_data.emplace_back((i << 2) + 3);
    }

    auto ib
        = create_indice_buffer(indice_data.size(), indice_data.data(), true);

    begin();

    set_blend_func(blend::src_alpha, blend::one_minus_src_alpha);
    set_indice_buffer(ib);
    set_render_state(BLEND | WRITE_RGBA);
    set_program(font_program);

    mat4x4 model, view, projection;

    int window_width, window_height;
    window_size(&window_width, &window_height);

    float eye[] = { 0, 0, 1 };
    float center[] = { 0, 0, 0 };
    float up[] = { 0, 1, 0 };
    mat4x4_look_at(view, eye, center, up);
    set_view_transform(view);

    mat4x4_ortho(projection, 0, window_width, 0, window_height, 0.1, 50.0);
    set_projection_transform(projection);

    mat4x4_identity(model);

    for (auto& item : vdmap) {
        set_model_transform(model);
        set_texture(0, item.first);
        set_vertex_buffer(create_vertex_buffer(
            vertex_layout(vertex_attr_type::position, vertex_attr_type::uv),
            item.second.size() / 6, item.second.data(), true));
        draw_element(0, 0, item.second.size() >> 2);
    }

    end();

    unused(x, y, ib, model, pen_x, pen_y);
}

} // namespace nx::gfx