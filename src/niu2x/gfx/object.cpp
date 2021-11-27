#include "gfx.h"

#include <string.h>

#include "niu2x/assert.h"
#include "niu2x/global.h"

#define STBRP_ASSERT(condition) NX_ASSERT(condition, "")

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#include <niu2x/fs.h>

#include "niu2x/utils.h"
#include "niu2x/list_head.h"
#include "niu2x/assert.h"
#include "niu2x/freelist.h"

#include "noto_scans_sc_regular.h"

namespace nx::gfx {

namespace {
struct object_type {
    enum { vertex_buffer, indice_buffer, program, texture, font };
};

} // namespace

#define create_object(list, type_t)                                            \
    {                                                                          \
        auto id = list.alloc();                                                \
        NX_ASSERT(id != list.nil, "too many object");                          \
        auto* obj = list.get(id);                                              \
        obj->type = object_type::type_t;                                       \
        obj->id = id;                                                          \
        obj;                                                                   \
    }

#define destroy_object(list, obj) list.free(obj->id);

static freelist<vertex_buffer_t, 1024> vertex_buffer_freelist;
static freelist<indice_buffer_t, 1024> indice_buffer_freelist;
static freelist<program_t, 1024> program_freelist;
static freelist<texture_t, 4096> texture_freelist;
static freelist<font_t, 64> font_freelist;

size_t vertex_sizeof(vertex_layout_t layout)
{
    size_t size = 0;
    for (int i = 0; i < 16; i++, layout >>= 4) {
        auto attr = (vertex_attr_type)(layout & 0xF);
        switch (attr) {
            case vertex_attr_type::nil: {
                break;
            }
            case vertex_attr_type::position: {
                // vec3
                size += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_type::color: {
                // vec4
                size += 4 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_type::normal: {
                // vec3
                size += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_type::uv: {
                // vec3
                size += 3 * sizeof(GLfloat);
                break;
            }
        }
    }
    return size;
}

vertex_buffer_t* create_vertex_buffer(
    vertex_layout_t layout, uint32_t vertices_count, void* data)
{
    auto* obj = (create_object(vertex_buffer_freelist, vertex_buffer));

    glGenBuffers(1, &(obj->name));
    obj->layout = layout;

    glBindBuffer(GL_ARRAY_BUFFER, obj->name);
    size_t buffer_size = vertex_sizeof(layout) * vertices_count;
    glBufferData(GL_ARRAY_BUFFER, buffer_size, data, GL_DYNAMIC_DRAW);

    NX_CHECK_GL_ERROR();
    return obj;
}

namespace {
struct font_private_data_t {
    stbtt_packedchar chardata[127 + 0x4DBF - 0x3400 + 1];
    stbtt_pack_context spc;
    texture_t* textures[1];
};

constexpr auto font_texture_width = 2048;
constexpr auto font_texture_height = 2048;
} // namespace

constexpr auto radius = 4;

struct font_t* create_builtin_font()
{
    auto* obj = (create_object(font_freelist, font));
    obj->private_data = global::mem.allocate(sizeof(font_private_data_t));
    NX_ASSERT(obj->private_data, "out of memory");

    struct font_private_data_t* font_data
        = reinterpret_cast<struct font_private_data_t*>(obj->private_data);

    std::vector<uint8_t> image;
    image.resize(font_texture_width * font_texture_height);

    stbtt_PackBegin(&(font_data->spc), image.data(), font_texture_width,
        font_texture_height, 0, 1, nullptr);

    stbtt_pack_range ranges[] = { {
                                      .font_size = 32,
                                      .first_unicode_codepoint_in_range = 1,
                                      .array_of_unicode_codepoints = nullptr,
                                      .num_chars = 127,
                                      .chardata_for_range = font_data->chardata,
                                      .h_oversample = 0,
                                      .v_oversample = 0,
                                  },
        {
            .font_size = 32,
            .first_unicode_codepoint_in_range = 0x3400,
            .array_of_unicode_codepoints = nullptr,
            .num_chars = 0x4DBF - 0x3400 + 1,
            .chardata_for_range = font_data->chardata,
            .h_oversample = 0,
            .v_oversample = 0,
        } };
    stbtt_PackFontRanges(
        &(font_data->spc), noto_scans_sc_regular, 0, ranges, 2);

    stbtt_PackEnd(&(font_data->spc));

    // std::vector<uint8_t> dstImage;
    // dstImage.resize(font_texture_width * font_texture_height);
    // auto r = 4.0;
    // auto s = 0.125f / (r * r);
    // for (auto y = 0; y < font_texture_height; y++) {
    //     for (auto x = 0; x < font_texture_width; x++) {
    //         auto index = y * font_texture_width + x;

    //         auto a0 = image.data()[index];
    //         dstImage[index] = image.data()[index];

    //         auto minDistSq = INT32_MAX;
    //         for (auto yy = -r; yy <= r; yy++) {
    //             for (auto xx = -r; xx <= r; xx++) {
    //                 auto xxx = xx + x;
    //                 if (xxx < 0 || xxx >= font_texture_width) {
    //                     continue;
    //                 }

    //                 auto yyy = yy + y;
    //                 if (yyy < 0 || yyy >= font_texture_height) {
    //                     continue;
    //                 }

    //                 auto a = image.data()[(int)(yyy * font_texture_width +
    //                 xxx)]; auto diff = abs(static_cast<int32_t>(a) - a0); if
    //                 (diff > 128) {
    //                     auto distSq = xx * xx + yy * yy;
    //                     if (minDistSq > distSq) {
    //                         minDistSq = distSq;
    //                     }
    //                 }
    //             }
    //         }

    //         int result = 0;
    //         if (minDistSq == INT32_MAX) {
    //             result = a0 < 128 ? 0x00 : 0xff;
    //         } else {
    //             auto distSq = sqrt(s * minDistSq);
    //             distSq = ((a0 < 128) ? -distSq : distSq) + 0.5f;
    //             result = static_cast<uint8_t>(round(255.0f * distSq));
    //         }

    //         dstImage[index + 0] = result;
    //     }
    // }

    unsigned char* ptr = nullptr;
    font_data->textures[0] = create_texture_2d(font_texture_width + 2 * radius,
        font_texture_height + 2 * radius, pixel_format::r8, ptr);

    free(ptr);

    return obj;
}

texture_t* font_texture(struct font_t* obj, int index)
{
    struct font_private_data_t* font_data
        = reinterpret_cast<struct font_private_data_t*>(obj->private_data);
    return font_data->textures[index];
}

indice_buffer_t* create_indice_buffer(uint32_t indices_count, void* data)
{

    auto* obj = (create_object(indice_buffer_freelist, indice_buffer));

    glGenBuffers(1, &(obj->name));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->name);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(indice_t),
        data, GL_DYNAMIC_DRAW);

    NX_CHECK_GL_ERROR();
    return obj;
}

static void destroy_indice_buffer(indice_buffer_t* obj)
{
    glDeleteBuffers(1, &(obj->name));
    destroy_object(indice_buffer_freelist, obj);
}

static void destroy_vertex_buffer(vertex_buffer_t* obj)
{
    glDeleteBuffers(1, &(obj->name));
    destroy_object(vertex_buffer_freelist, obj);
}

static GLuint create_shader(GLenum shader_type, const char* source_code)
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
static void destroy_shader(GLuint name) { glDeleteShader(name); }

static void destroy_texture(texture_t* obj)
{
    glDeleteTextures(1, &(obj->name));
    destroy_object(program_freelist, obj);
}

static void destroy_font(font_t* obj)
{
    struct font_private_data_t* font_data
        = reinterpret_cast<struct font_private_data_t*>(obj->private_data);
    destroy_texture(font_data->textures[0]);

    global::mem.free(obj->private_data);

    destroy_object(font_freelist, obj);
}

static GLuint create_program(GLuint vert, GLuint frag)
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
static void destroy_program(GLuint name) { glDeleteProgram(name); }

program_t* create_program(const char* vert, const char* frag)
{

    auto vert_name = create_shader(GL_VERTEX_SHADER, vert);
    NX_ASSERT(vert_name != 0, "vert compile fail");

    auto frag_name = create_shader(GL_FRAGMENT_SHADER, frag);
    NX_ASSERT(frag_name != 0, "frag compile fail");

    auto progame_name = create_program(vert_name, frag_name);
    NX_ASSERT(progame_name != 0, "program link fail");

    destroy_shader(vert_name);
    destroy_shader(frag_name);

    auto* obj = (create_object(program_freelist, program));
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

static void destroy_program(program_t* obj)
{

    destroy_program(obj->name);
    destroy_object(program_freelist, obj);
}

void destroy(object_t* obj)
{
#define CASE(enum_t, obj_t, destroy_method)                                    \
    case object_type::enum_t: {                                                \
        destroy_method(reinterpret_cast<obj_t*>(obj));                         \
        break;                                                                 \
    }

    switch (obj->type) {
        CASE(vertex_buffer, vertex_buffer_t, destroy_vertex_buffer)
        CASE(indice_buffer, indice_buffer_t, destroy_indice_buffer)
        CASE(program, program_t, destroy_program)
        CASE(texture, texture_t, destroy_texture)
        CASE(font, font_t, destroy_font)
    }

#undef CASE
}

vertex_layout_t vertex_layout(vertex_attr_type a0, vertex_attr_type a1,
    vertex_attr_type a2, vertex_attr_type a3, vertex_attr_type a4,
    vertex_attr_type a5, vertex_attr_type a6, vertex_attr_type a7,
    vertex_attr_type a8, vertex_attr_type a9, vertex_attr_type a10,
    vertex_attr_type a11, vertex_attr_type a12, vertex_attr_type a13,
    vertex_attr_type a14, vertex_attr_type a15)
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

texture_t* create_texture_2d_from_file(const char* pathname)
{
    auto file = fs::read_file(pathname);
    int w, h;
    int channels;
    auto* image
        = stbi_load_from_memory(file.data(), file.size(), &w, &h, &channels, 0);
    pixel_format pf;
    switch (channels) {
        case 1: {
            pf = pixel_format::r8;
            break;
        }
        case 3: {
            pf = pixel_format::rgb8;
            break;
        }
        case 4: {
            pf = pixel_format::rgba8;
            break;
        }
        default: {
            NX_ASSERT(
                false, "unsupport channels: %d (size: %dx%d)", channels, w, h);
        }
    }

    auto obj = create_texture_2d(w, h, pf, image);
    stbi_image_free(image);

    return obj;
}

texture_t* create_texture_2d(int w, int h, pixel_format pf, const void* data)
{
    auto* obj = (create_object(texture_freelist, texture));
    glGenTextures(1, &(obj->name));
    glBindTexture(GL_TEXTURE_2D, obj->name);
    switch (pf) {
        case pixel_format::rgba8: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, data);
            break;
        }
        case pixel_format::rgb8: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB,
                GL_UNSIGNED_BYTE, data);
            break;
        }
        case pixel_format::r8: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED,
                GL_UNSIGNED_BYTE, data);
            break;
        }
        default: {
            NX_ASSERT(false, "unsopport format: %d", pf);
        }
    }
    NX_CHECK_GL_ERROR();

    //
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    // GL_TEXTURE_WRAP_S
    // GL_TEXTURE_WRAP_T
    // GL_TEXTURE_WRAP_R
    obj->width = w;
    obj->height = h;
    NX_CHECK_GL_ERROR();
    return obj;
}

} // namespace nx::gfx
