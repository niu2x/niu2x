#ifndef NX_GFX_GFX_H
#define NX_GFX_GFX_H

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftadvanc.h>

#include <niu2x/gfx.h>

#include "niu2x/hashtab.h"

namespace nx::gfx {

using indice_t = uint32_t;

size_t vertex_sizeof(vertex_layout_t layout);

GLint program_uniform_location(struct program_t* obj, const char* name);

#define NX_CHECK_GL_ERROR()                                                    \
    do {                                                                       \
        auto e = glGetError();                                                 \
        NX_ASSERT(e == GL_NO_ERROR, "opengl error: %x", e);                    \
    } while (false);

void setup();
void cleanup();

extern font_t* default_font;
extern program_t* font_program;

extern vertex_buffer_t* sprite_vb;
extern indice_buffer_t* sprite_ib;

void window_size(int* w, int* h);

void auto_destroy_objects();

#define FONT_ALTAS_TEXTURE_SIZE 1024

namespace font {

void font_system_setup();
void font_system_cleanup();

struct font_altas_t {
    enum { PAGES_CAPACITY = 32 };
    struct texture_t* pages[PAGES_CAPACITY];
    struct hashtab_t charmap;
    int size;
    FT_Face face;
    int font_size;
    int cell_edge;
    int cell_capacity;
    int cell_size;
    int cell_row;
    int cell_col;
};

struct char_info_t {
    int advance;
    int page;
    struct hashtab_entry_t hash;

    int x, y, w, h;
    int xi, yi;
};

void font_altas_setup(font_altas_t* self, int font_size);
void font_altas_cleanup(font_altas_t* self);
char_info_t* font_altas_char_info(font_altas_t* self, uint32_t code);
int font_altas_kerning(font_altas_t* self, uint32_t left, uint32_t right);
} // namespace font

void mesh_init_from_file(mesh_t* mesh, const char* file, int idx, int flags);

} // namespace nx::gfx

#endif