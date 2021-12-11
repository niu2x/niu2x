#include <stdarg.h>

#include <vector>
#include <map>
#include <locale>
#include <codecvt>

#include "gfx.h"

namespace nx::gfx {

void printf(int x, int y, const char* fmt, ...)
{
    char buffer[1024];

    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, 1024, fmt, args);
    va_end(args);

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    std::u32string u32sz = conv.from_bytes(buffer);

    std::map<texture_t*, std::vector<float>> vertex_data_map;

    int pen_x = 0;
    int pen_y = 0;

    const char32_t* ptr = u32sz.c_str();
    ;

    size_t max_vertex_count = 0;

    while (*ptr) {
        auto ci = font_char_info(default_font, *ptr);

        if (vertex_data_map.find(ci.texture) == vertex_data_map.end()) {
            vertex_data_map[ci.texture] = std::vector<float>();
        }

        vertex_data_map[ci.texture].emplace_back(pen_x + ci.x);
        vertex_data_map[ci.texture].emplace_back(pen_y + ci.y);
        vertex_data_map[ci.texture].emplace_back(0);
        vertex_data_map[ci.texture].emplace_back(ci.uv_x);
        vertex_data_map[ci.texture].emplace_back(ci.uv_y + ci.uv_h);
        vertex_data_map[ci.texture].emplace_back(0);

        vertex_data_map[ci.texture].emplace_back(pen_x + ci.x + ci.w);
        vertex_data_map[ci.texture].emplace_back(pen_y + ci.y);
        vertex_data_map[ci.texture].emplace_back(0);
        vertex_data_map[ci.texture].emplace_back(ci.uv_x + ci.uv_w);
        vertex_data_map[ci.texture].emplace_back(ci.uv_y + ci.uv_h);
        vertex_data_map[ci.texture].emplace_back(0);

        vertex_data_map[ci.texture].emplace_back(pen_x + ci.x + ci.w);
        vertex_data_map[ci.texture].emplace_back(pen_y + ci.y + ci.h);
        vertex_data_map[ci.texture].emplace_back(0);
        vertex_data_map[ci.texture].emplace_back(ci.uv_x + ci.uv_w);
        vertex_data_map[ci.texture].emplace_back(ci.uv_y);
        vertex_data_map[ci.texture].emplace_back(0);

        vertex_data_map[ci.texture].emplace_back(pen_x + ci.x);
        vertex_data_map[ci.texture].emplace_back(pen_y + ci.y + ci.h);
        vertex_data_map[ci.texture].emplace_back(0);
        vertex_data_map[ci.texture].emplace_back(ci.uv_x);
        vertex_data_map[ci.texture].emplace_back(ci.uv_y);
        vertex_data_map[ci.texture].emplace_back(0);

        max_vertex_count
            = std::max(max_vertex_count, vertex_data_map[ci.texture].size());

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

    math::mat4x4_identity(model);
    math::mat4x4_identity(view);
    math::mat4x4_identity(projection);

    float eye[] = { 0, 0, 1 };
    float center[] = { 0, 0, 0 };
    float up[] = { 0, 1, 0 };
    math::mat4x4_look_at(view, eye, center, up);
    set_view_transform(view);

    math::mat4x4_ortho(
        projection, 0, window_size[0], 0, window_size[1], 0.1, 50.0);
    set_projection_transform(projection);

    math::mat4x4_translate(model, x, y, 0);

    auto layout
        = vertex_layout_build(vertex_attr_type::position, vertex_attr_type::uv);

    for (auto& item : vertex_data_map) {
        set_model_transform(model);
        set_texture(0, item.first);

        auto vertex_count = item.second.size() / 6;
        auto vertex_data = item.second.data();
        auto vb = create_vertex_buffer(layout, vertex_count, vertex_data, true);
        set_vertex_buffer(vb);
        draw_element(7, 0, item.second.size() >> 2);
    }

    end();

}

} // namespace nx::gfx