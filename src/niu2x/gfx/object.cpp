#include "gfx.h"

#include <string.h>

#include "niu2x/assert.h"
#include "niu2x/global.h"

#define STBRP_ASSERT(condition) NX_ASSERT(condition, "")

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <niu2x/fs.h>

#include "niu2x/utils.h"
#include "niu2x/list.h"
#include "niu2x/assert.h"
#include "niu2x/freelist.h"

namespace nx::gfx {

namespace {

struct object_type {
    enum {
        vertex_buffer,
        indice_buffer,
        program,
        texture,
        font,
        mesh,
        framebuffer,
        mesh_group,
    };
};

} // namespace

#define object_create(freelist, type_t)                                        \
    {                                                                          \
        auto id = freelist.alloc();                                            \
        NX_ASSERT(id != freelist.nil, "too many object");                      \
        auto* obj = freelist.get(id);                                          \
        obj->type = object_type::type_t;                                       \
        obj->id = id;                                                          \
        list_init(&(obj->list));                                               \
        obj;                                                                   \
    }

#define object_destroy(freelist, obj)                                          \
    freelist.free(obj->id);                                                    \
    list_del_init(&(obj->list));

static freelist<vertex_buffer_t, 4096> vertex_buffer_freelist;
static freelist<indice_buffer_t, 4096> indice_buffer_freelist;
static freelist<program_t, 1024> program_freelist;
static freelist<texture_t, 4096> texture_freelist;
static freelist<framebuffer_t, 256> framebuffer_freelist;
static freelist<font_t, 256> font_freelist;
static freelist<mesh_t, 1024> mesh_freelist;
static freelist<mesh_group_t, 1024> mesh_group_freelist;
static NX_LIST(auto_destroy_head);

static void framebuffer_destroy(framebuffer_t* obj)
{
    glDeleteRenderbuffers(1, &(obj->stencil_depth));
    glDeleteFramebuffers(1, &(obj->name));
    object_destroy(framebuffer_freelist, obj);
}

framebuffer_t* framebuffer_create(texture_t* texture)
{
    auto* obj = (object_create(framebuffer_freelist, framebuffer));

    glGenFramebuffers(1, &(obj->name));
    glBindFramebuffer(GL_FRAMEBUFFER, obj->name);

    glBindTexture(GL_TEXTURE_2D, texture->name);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->name, 0);

    glGenRenderbuffers(1, &(obj->stencil_depth));
    glBindRenderbuffer(GL_RENDERBUFFER, obj->stencil_depth);
    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, texture->width, texture->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, obj->stencil_depth);

    NX_ASSERT(
        glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
        "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    NX_CHECK_GL_ERROR();
    return obj;
}

size_t vertex_sizeof(vertex_layout_t layout)
{
    size_t size = 0;
    for (int i = 0; i < 16; i++, layout >>= 4) {
        auto attr = (vertex_attr_t)(layout & 0xF);
        switch (attr) {
            case vertex_attr_t::nil: {
                break;
            }
            case vertex_attr_t::position: {
                // vec3
                size += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_t::color: {
                // vec4
                size += 4 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_t::normal: {
                // vec3
                size += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_t::uv: {
                // vec3
                size += 3 * sizeof(GLfloat);
                break;
            }
        }
    }
    return size;
}

mesh_group_t* mesh_group_create_from_file(const char* path, int flags)
{
    auto* obj = (object_create(mesh_group_freelist, mesh_group));
    obj->texture = 0;
    mesh_group_init_from_file(obj, path, flags);
    return obj;
}

mesh_t* mesh_create_from_file(const char* path, int idx, int flags)
{
    auto* obj = (object_create(mesh_freelist, mesh));
    obj->texture = 0;
    mesh_init_from_file(obj, path, idx, flags);
    return obj;
}

// void set_view(layer_t layer, texture_t * texture) {

// }

vertex_buffer_t* vertex_buffer_create(vertex_layout_t layout,
    uint32_t vertices_count, void* data, bool auto_destroy)
{
    auto* obj = (object_create(vertex_buffer_freelist, vertex_buffer));
    obj->size = vertices_count;
    glGenBuffers(1, &(obj->name));
    obj->layout = layout;

    if (auto_destroy) {
        list_add(&(obj->list), &auto_destroy_head);
    }

    glBindBuffer(GL_ARRAY_BUFFER, obj->name);
    size_t buffer_size = vertex_sizeof(layout) * vertices_count;
    glBufferData(GL_ARRAY_BUFFER, buffer_size, data, GL_DYNAMIC_DRAW);

    NX_CHECK_GL_ERROR();
    return obj;
}

font_t* font_create(int font_size)
{
    auto* obj = (object_create(font_freelist, font));
    obj->private_data = NX_ALLOC(struct font::font_altas_t, 1);
    font::font_altas_setup((font::font_altas_t*)(obj->private_data), font_size);

    return obj;
}

void auto_destroy_objects() { 
    while(!list_empty(&auto_destroy_head)){
        object_t *obj = NX_LIST_ENTRY(auto_destroy_head.next, object_t, list);
        destroy(obj);
    }
}

indice_buffer_t* indice_buffer_create(
    uint32_t indices_count, void* data, bool auto_destroy)
{

    auto* obj = (object_create(indice_buffer_freelist, indice_buffer));
    obj->size = indices_count;

    if (auto_destroy) {
        list_add(&(obj->list), &auto_destroy_head);
    }

    glGenBuffers(1, &(obj->name));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->name);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(indice_t),
        data, GL_DYNAMIC_DRAW);

    NX_CHECK_GL_ERROR();
    return obj;
}

static void indice_buffer_destroy(indice_buffer_t* obj)
{
    glDeleteBuffers(1, &(obj->name));
    object_destroy(indice_buffer_freelist, obj);
}

static void vertex_buffer_destroy(vertex_buffer_t* obj)
{
    glDeleteBuffers(1, &(obj->name));
    object_destroy(vertex_buffer_freelist, obj);
}

font_char_info_t font_char_info(font_t* self, uint32_t code)
{
    auto* font_altas = (font::font_altas_t*)(self->private_data);

    auto* ci = font::font_altas_char_info(font_altas, code);

    return {
        .texture = font_altas->pages[ci->page],
        .uv_x = (float)ci->xi * font_altas->cell_edge / FONT_ALTAS_TEXTURE_SIZE,
        .uv_y = (float)ci->yi * font_altas->cell_edge / FONT_ALTAS_TEXTURE_SIZE,
        .uv_w = (float)ci->w / FONT_ALTAS_TEXTURE_SIZE,
        .uv_h = (float)ci->h / FONT_ALTAS_TEXTURE_SIZE,
        .x = ci->x,
        .y = ci->y,
        .w = ci->w,
        .h = ci->h,
        .advance = ci->advance,
    };
}

int font_kerning(font_t* self, uint32_t left, uint32_t right)
{
    auto* font_altas = (font::font_altas_t*)(self->private_data);
    return font::font_altas_kerning(font_altas, left, right);
}

static GLuint shader_create(GLenum shader_type, const char* source_code)
{
    auto name = glCreateShader(shader_type);
    glShaderSource(name, 1, &source_code, nullptr);
    glCompileShader(name);
    GLint status;
    glGetShaderiv(name, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLint max_len;
        glGetShaderiv(name, GL_INFO_LOG_LENGTH, &max_len);
        std::vector<GLchar> log(max_len);
        glGetShaderInfoLog(name, max_len, &max_len, &log[0]);
        NX_LOG_E("shader compile failed: %s", log.data());
        glDeleteShader(name);
        name = 0;
    }
    NX_CHECK_GL_ERROR();
    return name;
}
static void shader_destroy(GLuint name) { glDeleteShader(name); }

static void texture_destroy(texture_t* obj)
{
    if (obj->fb) {
        destroy(obj->fb);
        obj->fb = nullptr;
    }

    glDeleteTextures(1, &(obj->name));
    object_destroy(texture_freelist, obj);
}

static void font_destroy(font_t* obj)
{
    font::font_altas_cleanup((font::font_altas_t*)(obj->private_data));
    NX_FREE(obj->private_data);
    object_destroy(font_freelist, obj);
}

static GLuint program_create(GLuint vert, GLuint frag)
{
    auto name = glCreateProgram();
    glAttachShader(name, vert);
    glAttachShader(name, frag);

    glLinkProgram(name);

    GLint status = 0;
    glGetProgramiv(name, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        GLint max_len = 0;
        glGetProgramiv(name, GL_INFO_LOG_LENGTH, &max_len);
        std::vector<GLchar> log(max_len);
        glGetProgramInfoLog(name, max_len, &max_len, &log[0]);
        NX_LOG_E("program link failed: %s", log.data());
        glDeleteProgram(name);
        name = 0;
    }
    NX_CHECK_GL_ERROR();
    return name;
}
static void program_destroy(GLuint name) { glDeleteProgram(name); }

program_t* program_create(const char* vert, const char* frag)
{

    auto vert_name = shader_create(GL_VERTEX_SHADER, vert);
    NX_ASSERT(vert_name != 0, "vert compile fail");

    auto frag_name = shader_create(GL_FRAGMENT_SHADER, frag);
    NX_ASSERT(frag_name != 0, "frag compile fail");

    auto progame_name = program_create(vert_name, frag_name);
    NX_ASSERT(progame_name != 0, "program link fail");

    shader_destroy(vert_name);
    shader_destroy(frag_name);

    auto* obj = (object_create(program_freelist, program));
    obj->name = progame_name;

    GLint uniforms_size;
    glGetProgramiv(obj->name, GL_ACTIVE_UNIFORMS, &uniforms_size);
    NX_ASSERT(uniforms_size <= limits::max_uniform, "too many uniform");
    obj->uniforms_size = uniforms_size;


    for (int i = 0; i < uniforms_size; i++) {
        auto* uniform = &(obj->uniforms[i]);
        glGetActiveUniform(obj->name, i, limits::max_uniform_name, nullptr,
            &(uniform->size), &(uniform->type), uniform->name);
        uniform->location = glGetUniformLocation(obj->name, uniform->name);
        NX_ASSERT(
            uniform->location != -1, "no such uniform: %s", uniform->name);
    }

    NX_CHECK_GL_ERROR();
    return obj;
}

GLint program_uniform_location(struct program_t* obj, const char* name)
{
    for (int i = 0; i < obj->uniforms_size; i++) {
        if (strcmp(obj->uniforms[i].name, name) == 0)
            return obj->uniforms[i].location;
    }
    return -1;
}

static void program_destroy(program_t* obj)
{

    program_destroy(obj->name);
    object_destroy(program_freelist, obj);
}

static void mesh_destroy(mesh_t* obj)
{
    destroy(obj->vb);
    destroy(obj->ib);
    object_destroy(mesh_freelist, obj);
}

static void mesh_group_destroy_node(mesh_group_t::node_t* node)
{
    for (uint32_t i = 0; i < node->children_size; i++) {
        mesh_group_destroy_node(&(node->children[i]));
    }
    NX_FREE(node->children);
    NX_FREE(node->meshes);
}

mesh_t* mesh_create()
{
    auto* obj = (object_create(mesh_freelist, mesh));
    obj->texture = 0;
    obj->ib = nullptr;
    obj->vb = nullptr;
    return obj;
}

static void mesh_group_destroy(mesh_group_t* obj)
{
    for (mesh_t** ptr = obj->meshes; *ptr; ptr++) {
        destroy(*ptr);
    }
    NX_FREE(obj->meshes);
    
    mesh_group_destroy_node(&(obj->root));
    object_destroy(mesh_freelist, obj);
}

void destroy(object_t* obj)
{
    if (!obj)
        return;

#define CASE(enum_t, obj_t, destroy_method)                                    \
    case object_type::enum_t: {                                                \
        destroy_method(reinterpret_cast<obj_t*>(obj));                         \
        break;                                                                 \
    }

    switch (obj->type) {
        CASE(vertex_buffer, vertex_buffer_t, vertex_buffer_destroy)
        CASE(indice_buffer, indice_buffer_t, indice_buffer_destroy)
        CASE(program, program_t, program_destroy)
        CASE(texture, texture_t, texture_destroy)
        CASE(font, font_t, font_destroy)
        CASE(mesh, mesh_t, mesh_destroy)
        CASE(framebuffer, framebuffer_t, framebuffer_destroy)
        CASE(mesh_group, mesh_group_t, mesh_group_destroy)
    }

#undef CASE
}

vertex_layout_t vertex_layout(vertex_attr_t a0, vertex_attr_t a1, vertex_attr_t a2,
    vertex_attr_t a3, vertex_attr_t a4, vertex_attr_t a5, vertex_attr_t a6,
    vertex_attr_t a7, vertex_attr_t a8, vertex_attr_t a9, vertex_attr_t a10,
    vertex_attr_t a11, vertex_attr_t a12, vertex_attr_t a13, vertex_attr_t a14,
    vertex_attr_t a15)
{
    return (((vertex_layout_t)(a0)&0xF) << (4 * 0))
        | (((vertex_layout_t)(a1)&0xF) << (4 * 1))
        | (((vertex_layout_t)(a2)&0xF) << (4 * 2))
        | (((vertex_layout_t)(a3)&0xF) << (4 * 3))
        | (((vertex_layout_t)(a4)&0xF) << (4 * 4))
        | (((vertex_layout_t)(a5)&0xF) << (4 * 5))
        | (((vertex_layout_t)(a6)&0xF) << (4 * 6))
        | (((vertex_layout_t)(a7)&0xF) << (4 * 7))
        | (((vertex_layout_t)(a8)&0xF) << (4 * 8))
        | (((vertex_layout_t)(a9)&0xF) << (4 * 9))
        | (((vertex_layout_t)(a10)&0xF) << (4 * 10))
        | (((vertex_layout_t)(a11)&0xF) << (4 * 11))
        | (((vertex_layout_t)(a12)&0xF) << (4 * 12))
        | (((vertex_layout_t)(a13)&0xF) << (4 * 13))
        | (((vertex_layout_t)(a14)&0xF) << (4 * 14))
        | (((vertex_layout_t)(a15)&0xF) << (4 * 15));
}

texture_t* texture_create_from_file(const char* pathname)
{
    auto file = fs::read_file(pathname);
    int w, h;
    int channels;
    auto* image
        = stbi_load_from_memory(file.data(), file.size(), &w, &h, &channels, 0);
    pixel_format_t pf;
    switch (channels) {
        case 1: {
            pf = pixel_format_t::r8;
            break;
        }
        case 3: {
            pf = pixel_format_t::rgb8;
            break;
        }
        case 4: {
            pf = pixel_format_t::rgba8;
            break;
        }
        default: {
            NX_ASSERT(
                false, "unsupport channels: %d (size: %dx%d)", channels, w, h);
        }
    }

    auto obj = texture_create(w, h, pf, image);
    stbi_image_free(image);

    return obj;
}

void texture_update_region(
    texture_t* obj, int x, int y, int w, int h, const void* data)
{

    glBindTexture(GL_TEXTURE_2D, obj->name);
    switch (obj->pf) {
        case pixel_format_t::rgba8: {
            glTexSubImage2D(
                GL_TEXTURE_2D, 0, x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        }
        case pixel_format_t::rgb8: {
            glTexSubImage2D(
                GL_TEXTURE_2D, 0, x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        }
        case pixel_format_t::r8: {
            glTexSubImage2D(
                GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_UNSIGNED_BYTE, data);
            break;
        }
        default: {
            NX_ASSERT(false, "unsopport format: %d", obj->pf);
        }
    }
    NX_CHECK_GL_ERROR();

    glGenerateMipmap(GL_TEXTURE_2D);
    NX_CHECK_GL_ERROR();

    glBindTexture(GL_TEXTURE_2D, 0);
}

texture_t* texture_create(int w, int h, pixel_format_t pf, const void* data)
{
    auto* obj = (object_create(texture_freelist, texture));
    glGenTextures(1, &(obj->name));
    glBindTexture(GL_TEXTURE_2D, obj->name);
    switch (pf) {
        case pixel_format_t::rgba8: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, data);
            break;
        }
        case pixel_format_t::rgb8: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB,
                GL_UNSIGNED_BYTE, data);
            break;
        }
        case pixel_format_t::r8: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED,
                GL_UNSIGNED_BYTE, data);
            break;
        }
        default: {
            NX_ASSERT(false, "unsopport format: %d", pf);
        }
    }
    NX_CHECK_GL_ERROR();

    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    obj->width = w;
    obj->height = h;
    obj->pf = pf;
    obj->fb = nullptr;
    NX_CHECK_GL_ERROR();

    glBindTexture(GL_TEXTURE_2D, 0);

    return obj;
}

NXAPI framebuffer_t* texture_framebuffer(texture_t* obj)
{
    if (obj->fb == nullptr)
        obj->fb = framebuffer_create(obj);
    return obj->fb;
}

} // namespace nx::gfx
