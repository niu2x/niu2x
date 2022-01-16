#include "gfx.h"

#include <string.h>

#include "niu2x/utils.h"
#include "niu2x/list.h"
#include "niu2x/assert.h"

namespace nx::gfx {

enum class cmdtype {
    clear,
    draw_array,
    draw_element,
};

static constexpr int cmds_count = 4096;
static constexpr int max_cmd_textures = 8;
static struct cmd_t {
    texture_t* textures[max_cmd_textures];
    mat4x4 model;
    list_t list;
    struct {
        vertex_buffer_t* vbo;
        indice_buffer_t* ibo;
    };
    program_t* program;
    cmdtype type;
    uint32_t start;
    uint32_t count;

    union {
        struct {
            render_state_t render_state;

            blend bf_src;
            blend bf_dst;

            comparator stencil_comparator;
            uint8_t stencil_mask;
            uint8_t stencil_ref;
            stencil_op stencil_op_sfail;
            stencil_op stencil_op_dpfail;
            stencil_op stencil_op_dppass;
        };

        struct {
            uint32_t reversed[3];
        } render_state_md5;
    };

    int environment_idx;
} cmds[cmds_count];

// static uint64_t render_state_md5(cmd_t* cmd)
// {
// #define T(name) (uint64_t)(cmd->name)
//         // return T(render_state) ^ T(bf_src) ^ T(bf_dst) ^
//         // T(stencil_comparator)
//         //     ^ T(stencil_mask) ^ T(stencil_ref) ^ T(stencil_op_sfail)
//         //     ^ T(stencil_op_dpfail) ^ T(stencil_op_dppass);
//     return T(render_state_region[0] ) ^ T(render_state_region[1])
//         ^ T(render_state_region[2]);
// #undef T
// }

struct environment_t {
    mat4x4 view;
    mat4x4 projection;
    mat4x4 vp;
    bool vp_dirty;
};

struct builder_t {
    environment_t env;
    cmd_t cmd;
};

static constexpr int cmd_builder_queue_capacity = 16;
static builder_t builders[cmd_builder_queue_capacity];
static builder_t* current_builder = &builders[0];
static int builders_size = 1;

static constexpr int environments_capacity = 1024;
static environment_t environments[environments_capacity];
static int environments_size = 0;

static int next_cmd_idx = 0;

#define CHECK_CMD_COUNT()                                                      \
    NX_ASSERT(next_cmd_idx < cmds_count, "too many commands");

static void handle_cmd_clear(cmd_t*);
static void handle_cmd_draw_array(cmd_t* cmd);
static void handle_cmd_draw_element(cmd_t* cmd);
static void handle_render_state(cmd_t* cmd);

static constexpr int renderlayers_count = 16;

static struct renderlayer_t {
    list_t cmd_list;
    texture_t* view;
} layers[renderlayers_count];

static GLuint vao;

void render_setup()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void render_cleanup()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}

static void push()
{
    NX_ASSERT(builders_size < cmd_builder_queue_capacity, "");
    NX_ASSERT(environments_size < environments_capacity, "");
    current_builder = &builders[builders_size++];
    current_builder->env = (current_builder - 1)->env;
    reset();
    current_builder->cmd.environment_idx = environments_size++;
}

static void pop()
{
    NX_ASSERT(builders_size >= 2, "");
    environments[current_builder->cmd.environment_idx] = current_builder->env;
    current_builder = &builders[--builders_size - 1];
}

void begin()
{
    if (builders_size == 1) {

        environments_size = 0;
        next_cmd_idx = 0;

        for (int i = 0; i < renderlayers_count; i++) {
            layers[i].cmd_list = {
                &(layers[i].cmd_list),
                &(layers[i].cmd_list),
            };
            layers[i].view = nullptr;
        }
    }
    push();
}

static void update_environment(environment_t* env)
{
    if (env->vp_dirty) {
        env->vp_dirty = false;
        math::mat4x4_dup(env->vp, env->view);
        math::mat4x4_mul(env->vp, env->projection);
    }
}

static void draw_layer(renderlayer_t& layer)
{
    int env = -1;
    cmd_t* cmd;

    if (layer.view) {
        glBindFramebuffer(
            GL_DRAW_FRAMEBUFFER, texture_framebuffer(layer.view)->name);
        glViewport(0, 0, layer.view->width, layer.view->height);

    } else {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, window_size[0], window_size[1]);
    }

    NX_LIST_FOR_EACH(ptr, &(layer.cmd_list))
    {
        cmd = NX_LIST_ENTRY(ptr, struct cmd_t, list);

        auto this_env = cmd->environment_idx;
        if (env != this_env) {
            env = this_env;
            update_environment(&(environments[env]));
        }

        switch (cmd->type) {
            case cmdtype::clear: {
                handle_cmd_clear(cmd);
                break;
            }
            case cmdtype::draw_array: {
                handle_cmd_draw_array(cmd);
                break;
            }
            case cmdtype::draw_element: {
                handle_cmd_draw_element(cmd);
                break;
            }
        }
    }

    if (layer.view) {
        glBindTexture(GL_TEXTURE_2D, layer.view->name);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

static void draw_all()
{
    glBindVertexArray(vao);
    for (int i = renderlayers_count - 1; i >= 0; --i) {

        auto& layer = layers[i];
        draw_layer(layer);
    }
    glBindVertexArray(0);
    NX_CHECK_GL_ERROR();
}

void end()
{
    pop();

    if (builders_size == 1) {
        draw_all();
        auto_destroy_objects();
    }
}

static GLuint blend_func_map[] = {
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_ONE,
    GL_ZERO,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
};

void reset()
{
    auto& cmd = current_builder->cmd;
    auto environment_idx = cmd.environment_idx;
    memset(&(cmd), 0, sizeof(cmd_t));
    cmd.environment_idx = environment_idx;
}

void clear(layer_t layer)
{
    CHECK_CMD_COUNT();

    current_builder->cmd.type = cmdtype::clear;

    auto* cmd = &cmds[next_cmd_idx++];
    *cmd = current_builder->cmd;

    list_add_tail(&(cmd->list), &(layers[layer].cmd_list));
}

void draw_array(layer_t layer, uint32_t start, uint32_t count)
{
    CHECK_CMD_COUNT();
    current_builder->cmd.type = cmdtype::draw_array;

    current_builder->cmd.start = start;
    current_builder->cmd.count = count;

    auto* cmd = &cmds[next_cmd_idx++];
    *cmd = current_builder->cmd;
    list_add_tail(&(cmd->list), &(layers[layer].cmd_list));
}

void set_clear_color(color_t color)
{
    glClearColor(color.rgba.r / 255.0, color.rgba.g / 255.0,
        color.rgba.b / 255.0, color.rgba.a / 255.0);
}

void set_vertex_buffer(vertex_buffer_t* vbo)
{
    (current_builder->cmd).vbo = vbo;
}

void set_indice_buffer(indice_buffer_t* ibo)
{
    (current_builder->cmd).ibo = ibo;
}

void set_program(program_t* program)
{
    (current_builder->cmd).program = program;
}

static void handle_cmd_clear(cmd_t* cmd)
{
    handle_render_state(cmd);
#define BIT(name) GL_##name##_BUFFER_BIT
    static constexpr auto all = BIT(COLOR) | BIT(DEPTH) | BIT(STENCIL);
#undef BIT
    glClear(all);
}

static void vertex_layout_active(vertex_layout_t layout)
{

    auto size = vertex_sizeof(layout);

    size_t offset = 0;
    for (int i = 0; i < 16; i++, layout >>= 4) {
        auto attr = (vertex_attr_t)(layout & 0xF);
        if (attr != vertex_attr_t::nil) {
            glEnableVertexAttribArray(i);
        } else {
            glDisableVertexAttribArray(i);
        }

        switch (attr) {
            case vertex_attr_t::nil: {
                break;
            }
            case vertex_attr_t::position: {
                // vec3
                glVertexAttribPointer(
                    i, 3, GL_FLOAT, GL_FALSE, size, (void*)offset);
                offset += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_t::color: {
                // vec4
                glVertexAttribPointer(
                    i, 4, GL_FLOAT, GL_FALSE, size, (void*)offset);
                offset += 4 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_t::normal: {
                // vec3
                glVertexAttribPointer(
                    i, 3, GL_FLOAT, GL_FALSE, size, (void*)offset);
                offset += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_t::uv: {
                // vec3
                glVertexAttribPointer(
                    i, 3, GL_FLOAT, GL_FALSE, size, (void*)offset);
                offset += 3 * sizeof(GLfloat);
                break;
            }
        }
    }
}

void draw_element(layer_t layer, uint32_t start, uint32_t count)
{
    CHECK_CMD_COUNT();
    current_builder->cmd.type = cmdtype::draw_element;

    current_builder->cmd.start = start;
    current_builder->cmd.count = count;
    auto* cmd = &cmds[next_cmd_idx++];
    *cmd = current_builder->cmd;
    list_add_tail(&(cmd->list), &(layers[layer].cmd_list));
}

static GLuint gl_comparator_map[] = {
    GL_EQUAL,
    GL_NEVER,
    GL_LESS,
    GL_LEQUAL,
    GL_GREATER,
    GL_GEQUAL,
    GL_NOTEQUAL,
    GL_ALWAYS,
};

static GLuint gl_stencil_op_map[] = {
    GL_KEEP,
    GL_ZERO,
    GL_REPLACE,
    GL_INCR,
    GL_INCR_WRAP,
    GL_DECR,
    GL_DECR_WRAP,
    GL_INVERT,
};

static GLuint gl_comparator(comparator cmp)
{
    return gl_comparator_map[(uint8_t)cmp];
}

static auto gl_stencil_op(stencil_op op)
{
    return gl_stencil_op_map[(uint8_t)op];
}

static void handle_render_state(cmd_t* cmd)
{
    // uint64_t rs_md5 = render_state_md5(cmd);

    // NX_LOG_D("rs_md5 %u", rs_md5);

    static decltype(cmd->render_state_md5) last_rs_md5 { 0 };

    if (memcmp(&last_rs_md5, &(cmd->render_state_md5), sizeof(last_rs_md5))) {
        memcpy(&last_rs_md5, &(cmd->render_state_md5), sizeof(last_rs_md5));

        auto rs = cmd->render_state;
        auto should_cull_front = (rs & CULL_FRONT);
        auto should_cull_back = (rs & CULL_BACK);
        if (should_cull_front || should_cull_back) {
            glEnable(GL_CULL_FACE);
            if (should_cull_front && should_cull_back) {
                glCullFace(GL_FRONT_AND_BACK);
            } else if (should_cull_front) {
                glCullFace(GL_FRONT);
            } else
                glCullFace(GL_BACK);
        } else {
            glDisable(GL_CULL_FACE);
        }

        GLboolean write_r = GL_FALSE;
        GLboolean write_g = GL_FALSE;
        GLboolean write_b = GL_FALSE;
        GLboolean write_a = GL_FALSE;

        if (rs & WRITE_R)
            write_r = GL_TRUE;
        if (rs & WRITE_G)
            write_g = GL_TRUE;
        if (rs & WRITE_B)
            write_b = GL_TRUE;
        if (rs & WRITE_A)
            write_a = GL_TRUE;

        glColorMask(write_r, write_g, write_b, write_a);

        glDepthMask((rs & WRITE_DEPTH) ? GL_TRUE : GL_FALSE);

        if (rs & WRITE_STENCIL)
            glStencilMask(cmd->stencil_mask);
        else
            glStencilMask(0);

        glStencilFunc(gl_comparator(cmd->stencil_comparator), cmd->stencil_ref,
            cmd->stencil_mask);

        glStencilOp(gl_stencil_op(cmd->stencil_op_sfail),
            gl_stencil_op(cmd->stencil_op_dpfail),
            gl_stencil_op(cmd->stencil_op_dppass));

        if (rs & DEPTH_TEST) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        if (rs & STENCIL_TEST) {
            glEnable(GL_STENCIL_TEST);

        } else {
            glDisable(GL_STENCIL_TEST);
        }

        if (rs & BLEND) {
            glEnable(GL_BLEND);
            glBlendFunc(blend_func_map[(uint8_t)(cmd->bf_src)],
                blend_func_map[(uint8_t)(cmd->bf_dst)]);
        } else
            glDisable(GL_BLEND);
    }
}

static void program_active(cmd_t* cmd)
{
    auto& env = environments[cmd->environment_idx];
    // NX_LOG_D("cmd->environment_idx %d %p", cmd->environment_idx, cmd);
    glUseProgram(cmd->program->name);
    auto mvp_location = program_uniform_location(cmd->program, "MVP");
    if (mvp_location != -1) {
        mat4x4 mvp;
        math::mat4x4_dup(mvp, cmd->model);
        math::mat4x4_mul(mvp, env.vp);
        glUniformMatrix4fv(mvp_location, 1, GL_TRUE, (const float*)(mvp));
    }

    auto mv_location = program_uniform_location(cmd->program, "MV");
    if (mv_location != -1) {
        mat4x4 mv;
        math::mat4x4_dup(mv, cmd->model);
        math::mat4x4_mul(mv, env.view);

        glUniformMatrix4fv(mv_location, 1, GL_TRUE, (const float*)(mv));
    }

    auto m_location = program_uniform_location(cmd->program, "M");
    if (m_location != -1) {
        glUniformMatrix4fv(m_location, 1, GL_TRUE, (const float*)(cmd->model));
    }

    auto v_location = program_uniform_location(cmd->program, "V");
    if (v_location != -1) {
        glUniformMatrix4fv(v_location, 1, GL_TRUE, (const float*)(env.view));
    }

    auto time_location = program_uniform_location(cmd->program, "TIME");
    if (time_location != -1) {
        glUniform1f(time_location, now_seconds);
    }

    char texture_uniform_name[] = "TEX0";
    for (int i = 0; i < max_cmd_textures; i++) {
        if (cmd->textures[i]) {
            texture_uniform_name[3] = i + '0';
            auto location
                = program_uniform_location(cmd->program, texture_uniform_name);
            if (location != -1) {
                glUniform1i(location, i);
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, cmd->textures[i]->name);
                // NX_LOG_D("bind text %d %u", i, cmd->textures[i]->name);
            }
        }
    }
}
void set_blend_func(blend src_func, blend dst_func)
{
    auto& cmd_builder = current_builder->cmd;
    cmd_builder.bf_src = src_func;
    cmd_builder.bf_dst = dst_func;
}

void set_texture(texture_id_t tex_id, texture_t* tex)
{
    NX_ASSERT(tex_id < max_cmd_textures, "too large tex_id %d", tex_id);

    auto& cmd_builder = current_builder->cmd;
    cmd_builder.textures[tex_id] = tex;
}

static void handle_cmd_draw_array(cmd_t* cmd)
{
    handle_render_state(cmd);
    glBindBuffer(GL_ARRAY_BUFFER, cmd->vbo->name);
    vertex_layout_active(cmd->vbo->layout);
    program_active(cmd);
    glDrawArrays(GL_TRIANGLES, cmd->start, cmd->count);
}

static void handle_cmd_draw_element(cmd_t* cmd)
{
    handle_render_state(cmd);
    glBindBuffer(GL_ARRAY_BUFFER, cmd->vbo->name);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmd->ibo->name);
    vertex_layout_active(cmd->vbo->layout);
    program_active(cmd);
    glDrawElements(GL_TRIANGLES, cmd->count, GL_UNSIGNED_INT,
        (void*)(cmd->start * sizeof(indice_t)));
}

void set_render_state(render_state_t rs)
{
    auto& cmd_builder = current_builder->cmd;
    cmd_builder.render_state = rs;
}

void set_model_transform(const mat4x4 m)
{
    auto& cmd_builder = current_builder->cmd;
    math::mat4x4_dup(cmd_builder.model, m);
}

void model_transform(mat4x4 output)
{
    auto& cmd_builder = current_builder->cmd;
    math::mat4x4_dup(output, cmd_builder.model);
}

void set_view_transform(const mat4x4 m)
{
    math::mat4x4_dup(current_builder->env.view, m);
    current_builder->env.vp_dirty = true;
}

void set_projection_transform(const mat4x4 m)
{
    math::mat4x4_dup(current_builder->env.projection, m);
    current_builder->env.vp_dirty = true;
}

void set_view(layer_t layer, texture_t* texture)
{
    auto& render_layer = layers[layer];
    render_layer.view = texture;
}

void set_stencil_func(comparator cmp, uint8_t ref, uint8_t mask)
{
    current_builder->cmd.stencil_comparator = cmp;
    current_builder->cmd.stencil_ref = ref;
    current_builder->cmd.stencil_mask = mask;
}

void set_stencil_op(stencil_op sfail, stencil_op dpfail, stencil_op dppass)
{
    current_builder->cmd.stencil_op_sfail = sfail;
    current_builder->cmd.stencil_op_dpfail = dpfail;
    current_builder->cmd.stencil_op_dppass = dppass;
}

} // namespace nx::gfx