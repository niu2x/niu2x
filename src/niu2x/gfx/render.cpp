#include "gfx.h"

#include <string.h>

#include "niu2x/utils.h"
#include "niu2x/list_head.h"
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
    mat4x4 model;
    list_head list;
    texture_t* textures[max_cmd_textures];
    vertex_buffer_t* vbo;
    indice_buffer_t* ibo;
    program_t* program;
    cmdtype type;
    render_state_t render_state;
    uint32_t start;
    uint32_t count;
    blend_t bf_src;
    blend_t bf_dst;
    int environment_idx;
} cmds[cmds_count];

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
static void handle_render_state(render_state_t rs);

static constexpr int renderlayers_count = 16;

static struct renderlayer_t {
    list_head cmd_list;
    framebuffer_t* framebuffer;
} layers[renderlayers_count];

void render_setup()
{
    for (auto& it : layers) {
        it.framebuffer = nullptr;
    }
}

void render_cleanup()
{
    for (auto& it : layers) {
        if (it.framebuffer) {
            destroy(it.framebuffer);
            it.framebuffer = nullptr;
        }
    }
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
        next_cmd_idx = 0;
        for (int i = 0; i < renderlayers_count; i++) {
            layers[i].cmd_list = {
                &(layers[i].cmd_list),
                &(layers[i].cmd_list),
            };
        }
    }
    push();
}

static void update_environment(environment_t* env)
{
    if (env->vp_dirty) {
        env->vp_dirty = false;
        math::mat4x4_mul(env->vp, env->view, env->projection);
    }
}

void end()
{
    pop();

    if (builders_size == 1) {
        {
            environment_t* env = nullptr;
            cmd_t* cmd;
            for (int i = 0; i < renderlayers_count; i++) {

                glBindFramebuffer(GL_FRAMEBUFFER,
                    layers[i].framebuffer ? layers[i].framebuffer->name : 0);

                NX_LIST_FOR_EACH(ptr, &(layers[i].cmd_list))
                {
                    cmd = NX_LIST_ENTRY(ptr, struct cmd_t, list);

                    auto* this_env = &(environments[cmd->environment_idx]);
                    if (env != this_env) {
                        env = this_env;
                        update_environment(env);
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
            }
            NX_CHECK_GL_ERROR();
        }

        {
            auto_destroy_objects();
            environments_size = 0;
        }
    }
}

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
    handle_render_state(cmd->render_state);
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
        auto attr = (vertex_attr_type)(layout & 0xF);

        if (attr != vertex_attr_type::nil) {
            glEnableVertexAttribArray(i);
        } else {
            glDisableVertexAttribArray(i);
        }

        switch (attr) {
            case vertex_attr_type::nil: {
                break;
            }
            case vertex_attr_type::position: {
                // vec3
                glVertexAttribPointer(
                    i, 3, GL_FLOAT, GL_FALSE, size, (void*)offset);
                offset += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_type::color: {
                // vec4
                glVertexAttribPointer(
                    i, 4, GL_FLOAT, GL_FALSE, size, (void*)offset);
                offset += 4 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_type::normal: {
                // vec3
                glVertexAttribPointer(
                    i, 3, GL_FLOAT, GL_FALSE, size, (void*)offset);
                offset += 3 * sizeof(GLfloat);
                break;
            }
            case vertex_attr_type::uv: {
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

    NX_LOG_D("i %d", current_builder->cmd.environment_idx);

    current_builder->cmd.start = start;
    current_builder->cmd.count = count;
    auto* cmd = &cmds[next_cmd_idx++];
    *cmd = current_builder->cmd;
    list_add_tail(&(cmd->list), &(layers[layer].cmd_list));
    NX_CHECK_GL_ERROR();
}

static void handle_render_state(render_state_t rs)
{
    static render_state_t last_render_state = (render_state_t)0;
    if (last_render_state != rs) {
        last_render_state = rs;

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
        glStencilMask((rs & WRITE_STENCIL) ? GL_TRUE : GL_FALSE);

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

        if (rs & BLEND)
            glEnable(GL_BLEND);
        else
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
        math::mat4x4_mul(mvp, cmd->model, env.vp);
        glUniformMatrix4fv(mvp_location, 1, GL_TRUE, (const float*)(mvp));
    }

    auto mv_location = program_uniform_location(cmd->program, "MV");
    if (mv_location != -1) {
        mat4x4 mv;
        math::mat4x4_mul(mv, cmd->model, env.view);
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
            }
        }
    }
}
void set_blend_func(uint8_t src_func, uint8_t dst_func)
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

static void handle_blend_func(cmd_t* cmd)
{
    if (cmd->render_state & BLEND) {
        glBlendFunc(blend_func_map[cmd->bf_src], blend_func_map[cmd->bf_dst]);
    }
}

static void handle_cmd_draw_array(cmd_t* cmd)
{
    handle_render_state(cmd->render_state);
    handle_blend_func(cmd);
    glBindBuffer(GL_ARRAY_BUFFER, cmd->vbo->name);
    vertex_layout_active(cmd->vbo->layout);
    program_active(cmd);
    glDrawArrays(GL_TRIANGLES, cmd->start, cmd->count);
}

static void handle_cmd_draw_element(cmd_t* cmd)
{
    handle_render_state(cmd->render_state);
    handle_blend_func(cmd);
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
    if (render_layer.framebuffer) {
        destroy(render_layer.framebuffer);
        render_layer.framebuffer = nullptr;
    }

    if (texture) {
        render_layer.framebuffer
            = create_framebuffer(texture->width, texture->height, texture);
    }
}

} // namespace nx::gfx