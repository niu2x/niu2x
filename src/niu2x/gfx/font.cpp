#include "gfx.h"

#include <niu2x/log.h>

#include "niu2x/gfx.h"
#include "noto_scans_sc_regular.h"

#define FONT_LOAD_FLAGS                                                        \
    (FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT)

namespace nx::gfx {
program_t* font_program = nullptr;
}

namespace nx::gfx::font {

static FT_Library ft_lib;
static FT_Face default_face;

static void ft_library_setup() { FT_Init_FreeType(&ft_lib); }

static void ft_library_cleanup() { FT_Done_FreeType(ft_lib); }

static FT_Face create_face(const void* ptr, size_t size)
{
    FT_Face face;
    FT_New_Memory_Face(ft_lib, (const uint8_t*)ptr, size, 0, &face);
    return face;
}

static void destroy_face(FT_Face face) { FT_Done_Face(face); }

static void font_altas_setup(font_altas_t* self, FT_Face face, int font_size)
{
    self->size = 0;
    self->face = face;
    self->font_size = font_size;

    FT_Set_Pixel_Sizes(self->face, self->font_size, self->font_size);

    auto& metrics = self->face->size->metrics;
    self->cell_edge = ((metrics.ascender - metrics.descender) >> 6) + 2;
    self->cell_size = 0;
    self->cell_row = FONT_ALTAS_TEXTURE_SIZE / self->cell_edge;
    self->cell_col = FONT_ALTAS_TEXTURE_SIZE / self->cell_edge;
    self->cell_capacity = self->cell_col * self->cell_row;

    NX_LOG_D("self->cell_edge %d", self->cell_edge);

    hashtab_setup(&(self->charmap));
}

void font_altas_setup(font_altas_t* self, int font_size)
{
    font_altas_setup(self, default_face, font_size);
}

void font_altas_cleanup(font_altas_t* self)
{

    struct hashtab_entry_t* ptr = hashtab_first(&(self->charmap));
    while (ptr) {
        auto* ci = NX_HASHTAB_ENTRY(ptr, struct char_info_t, hash);
        hashtab_del(ptr);
        NX_FREE(ci);
        ptr = hashtab_first(&(self->charmap));
    }

    for (int i = 0; i < self->size; i++)
        destroy(self->pages[i]);
    self->size = 0;

    hashtab_cleanup(&(self->charmap));
}

static void font_altas_increase_page(font_altas_t* self)
{
    NX_ASSERT(self->size < font_altas_t::PAGES_CAPACITY, "too many page");
    self->pages[self->size++] = create_texture_2d(FONT_ALTAS_TEXTURE_SIZE,
        FONT_ALTAS_TEXTURE_SIZE, pixel_format::r8, nullptr);
    self->cell_size = 0;
}

static void font_altas_ensure_capacity(font_altas_t* self)
{
    if (self->size == 0)
        font_altas_increase_page(self);
    else {
        if (self->cell_size == self->cell_capacity)
            font_altas_increase_page(self);
    }
}

static void font_altas_generate_glyph(font_altas_t* self, uint32_t code)
{
    FT_Set_Pixel_Sizes(self->face, self->font_size, self->font_size);
    auto glyph_index = FT_Get_Char_Index(self->face, code);

    if (FT_Load_Glyph(self->face, glyph_index, FONT_LOAD_FLAGS)) {
        return;
    }

    auto* slot = self->face->glyph;

    if (FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL))
        return;

    font_altas_ensure_capacity(self);

    auto* ci = NX_ALLOC(struct char_info_t, 1);
    ci->advance = (slot->advance.x) >> 6;

    ci->w = slot->bitmap.width;
    ci->h = slot->bitmap.rows;
    ci->x = slot->bitmap_left;
    ci->y = slot->bitmap_top - ci->h;

    auto idx = self->cell_size++;

    auto xi = idx % self->cell_col;
    auto yi = idx / self->cell_col;
    ci->xi = xi;
    ci->yi = yi;

    std::vector<uint8_t> buffer;
    buffer.resize(ci->w * ci->h);

    unsigned char* read_ptr = slot->bitmap.buffer;
    unsigned char* write_ptr = buffer.data();

    for (int i = 0; i < ci->h;
         i++, write_ptr += ci->w, read_ptr += slot->bitmap.pitch) {
        memcpy(write_ptr, read_ptr, ci->w);
    }

    texture_2d_update_region(self->pages[self->size - 1], xi * self->cell_edge,
        yi * self->cell_edge, ci->w, ci->h, buffer.data());
    ci->page = self->size - 1;

    hashtab_set(&(self->charmap), code, &(ci->hash));
}

char_info_t* font_altas_char_info(font_altas_t* self, uint32_t code)
{
    auto* ptr = hashtab_get(&(self->charmap), code);
    if (!ptr)
        font_altas_generate_glyph(self, code);
    ptr = hashtab_get(&(self->charmap), code);
    NX_ASSERT(ptr, "");

    auto* ci = NX_HASHTAB_ENTRY(ptr, struct char_info_t, hash);
    return ci;
}

static const char* font_program_source[] = { R"RAW(
#version 300 es

layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 uv;

uniform mat4 mvp;
out highp vec3 v_uv;

void main()
{
  gl_Position =  vec4(position, 1.0) * mvp;
  v_uv = uv;
}

)RAW",
    R"RAW(
#version 300 es

uniform sampler2D tex0;
in highp vec3 v_uv;

out highp vec4 color;

void main()
{
    highp float a = texture(tex0, v_uv.xy).r;
    color = vec4(vec3(1, 1, 1), a);
}

)RAW" };

void font_system_setup()
{
    ft_library_setup();
    default_face
        = create_face(noto_scans_sc_regular, noto_scans_sc_regular_length);

    font_program
        = create_program(font_program_source[0], font_program_source[1]);
}
void font_system_cleanup()
{
    destroy(font_program);
    destroy_face(default_face);
    ft_library_cleanup();
}

int font_altas_kerning(font_altas_t* self, uint32_t left, uint32_t right)
{
    FT_Set_Pixel_Sizes(self->face, self->font_size, self->font_size);

    auto li = FT_Get_Char_Index(self->face, left);
    auto ri = FT_Get_Char_Index(self->face, right);

    FT_Vector pos;
    FT_Get_Kerning(self->face, li, ri, 0, &pos);

    return pos.x >> 6;
}

}; // namespace nx::gfx::font

namespace nx::test {

void test_font_altas()
{
    gfx::font::font_altas_t font;
    gfx::font::font_altas_setup(&font, gfx::font::default_face, 24);

    gfx::font::font_altas_cleanup(&font);
}

} // namespace nx::test