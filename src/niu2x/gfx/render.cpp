#include <niu2x/gfx.h>

#include "niu2x/utils.h"
#include "niu2x/list_head.h"
#include "niu2x/assert.h"
#include "gfx.h"

namespace nx::gfx {

enum class cmdtype {
    clear,
    draw_array,
};

static constexpr int cmds_count = 4096;

static struct cmd_t {
    cmdtype type;
    list_head list;
    vertex_buffer_t* vbo;
    program_t* program;

    uint32_t start;
    uint32_t count;
} cmds[cmds_count];

static constexpr int cmd_builder_queue_capacity = 16;
static struct cmd_t cmd_builder_queue[cmd_builder_queue_capacity];
static int cmd_builder_queue_size = 0;

static int next_cmd_idx = 0;

#define CHECK_CMD_COUNT()                                                      \
    NX_ASSERT(next_cmd_idx < cmds_count, "too many commands");

static void handle_cmd_clear(cmd_t*);
static void handle_cmd_draw_array(cmd_t* cmd);

static constexpr int renderlayers_count = 16;

static struct renderlayer_t {
    list_head cmd_list;

} layers[renderlayers_count];

void begin()
{
    next_cmd_idx = 0;
    for (int i = 0; i < renderlayers_count; i++) {
        layers[i].cmd_list = {
            &(layers[i].cmd_list),
            &(layers[i].cmd_list),
        };
    }
}

void end()
{
    cmd_t* cmd;
    for (int i = renderlayers_count - 1; i >= 0; i--) {
        NX_LIST_FOR_EACH(ptr, &(layers[i].cmd_list))
        {
            cmd = NX_LIST_ENTRY(ptr, struct cmd_t, list);
            switch (cmd->type) {
                case cmdtype::clear: {
                    handle_cmd_clear(cmd);
                    break;
                }
                case cmdtype::draw_array: {
                    handle_cmd_draw_array(cmd);
                    break;
                }
            }
        }
    }
}

void clear(layer_t layer)
{
    CHECK_CMD_COUNT();

    auto& cmd_builder = cmd_builder_queue[cmd_builder_queue_size];
    cmd_builder.type = cmdtype::clear;

    auto* cmd = &cmds[next_cmd_idx++];
    *cmd = cmd_builder;

    list_add_tail(&(cmd->list), &(layers[layer].cmd_list));
}

void draw_array(layer_t layer, uint32_t start, uint32_t count)
{
    CHECK_CMD_COUNT();
    auto& cmd_builder = cmd_builder_queue[cmd_builder_queue_size];
    cmd_builder.type = cmdtype::draw_array;
    cmd_builder.start = start;
    cmd_builder.count = count;

    auto* cmd = &cmds[next_cmd_idx++];
    *cmd = cmd_builder;
    list_add_tail(&(cmd->list), &(layers[layer].cmd_list));
}

void set_clear_color(color_t color)
{
    glClearColor(color.rgba.r / 255.0, color.rgba.g / 255.0,
        color.rgba.b / 255.0, color.rgba.a / 255.0);
}

void set_vertex_buffer(vertex_buffer_t* vbo)
{
    auto& cmd_builder = cmd_builder_queue[cmd_builder_queue_size];
    cmd_builder.vbo = vbo;
}

void set_program(program_t* program)
{
    auto& cmd_builder = cmd_builder_queue[cmd_builder_queue_size];
    cmd_builder.program = program;
}

static void handle_cmd_clear(cmd_t* cmd)
{
    unused(cmd);
#define BIT(name) GL_##COLOR##_BUFFER_BIT
    static constexpr auto all = BIT(COLOR) | BIT(DEPTH) | BIT(STENCIL);
#undef BIT
    glClear(all);
}

static void vertex_layout_active(vertex_layout_t layout)
{

    auto size = vertex_sizeof(layout);

    NX_LOG_D("size %lu, %lu", layout, size);

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
                // vec4
                glVertexAttribPointer(
                    i, 4, GL_FLOAT, GL_FALSE, size, (void*)offset);
                offset += 4 * sizeof(GLfloat);
                break;
            }
        }
    }
}

static void handle_cmd_draw_array(cmd_t* cmd)
{
    glBindBuffer(GL_ARRAY_BUFFER, cmd->vbo->name);
    vertex_layout_active(cmd->vbo->layout);
    glUseProgram(cmd->program->name);
    glDrawArrays(GL_TRIANGLES, cmd->start, cmd->count);
    NX_LOG_D("draw_array");
}

} // namespace nx::gfx