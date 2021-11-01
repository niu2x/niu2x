#include <niu2x/gfx.h>

#include "niu2x/utils.h"
#include "niu2x/list_head.h"
#include "niu2x/assert.h"

namespace nx::gfx {

enum class cmdtype {
    clear,
};

static constexpr int cmds_count = 4096;

static struct cmd_t {
    cmdtype type;
    list_head list;

} cmds[cmds_count];

static int next_cmd_idx = 0;

#define CHECK_CMD_COUNT()                                                      \
    NX_ASSERT(next_cmd_idx < cmds_count, "too many commands");

static void handle_cmd_clear(cmd_t*);

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
            }
        }
    }
}

void clear(layer_t layer)
{
    CHECK_CMD_COUNT();
    auto* cmd = &cmds[next_cmd_idx++];
    cmd->type = cmdtype::clear;

    list_add_tail(&(cmd->list), &(layers[layer].cmd_list));
}

void set_clear_color(color_t color)
{
    glClearColor(color.rgba.r / 255.0, color.rgba.g / 255.0,
        color.rgba.b / 255.0, color.rgba.a / 255.0);
}

static void handle_cmd_clear(cmd_t* cmd)
{
    unused(cmd);
#define BIT(name) GL_##COLOR##_BUFFER_BIT
    static constexpr auto all = BIT(COLOR) | BIT(DEPTH) | BIT(STENCIL);
#undef BIT
    glClear(all);
}

} // namespace nx::gfx