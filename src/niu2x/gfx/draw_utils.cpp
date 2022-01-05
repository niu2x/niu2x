#include "gfx.h"

namespace nx::gfx {

void draw_texture(layer_t layer, texture_t* texture)
{
    static gfx::render_state_t render_state = gfx::WRITE_RGBA | gfx::BLEND;

    begin();

    gfx::reset();

    set_render_state(render_state);
    set_blend_func(blend::src_alpha, blend::one_minus_src_alpha);

    set_texture(0, texture);
    set_vertex_buffer(sprite_vb);
    set_indice_buffer(sprite_ib);
    set_program(sprite_program);

    mat4x4 model;
    math::mat4x4_identity(model);
    math::mat4x4_translate(model, 0.5, 0.5, 0);

    mat4x4 scale;
    math::mat4x4_identity(scale);
    math::mat4x4_scale_aniso(scale, window_size[0], window_size[1], 0);
    math::mat4x4_mul(model, scale);
    set_model_transform(model);

    mat4x4 view;
    vec3 eye = { 0, 0, 1 };
    vec3 center = { 0, 0, 0 };
    vec3 up = { 0, 1, 0 };
    math::mat4x4_look_at(view, eye, center, up);
    set_view_transform(view);

    mat4x4 projection;
    math::mat4x4_ortho(
        projection, 0, window_size[0], 0, window_size[1], 0.1, 244.0);
    set_projection_transform(projection);

    draw_element(layer, 0, 6);

    end();
}

void draw_mesh(layer_t layer, mesh_t* mesh)
{
    set_indice_buffer(mesh->ib);
    set_vertex_buffer(mesh->vb);
    set_texture(0, mesh->texture);
    draw_element(layer, 0, mesh->ib->size);
}

static void draw_mesh_group_node(layer_t layer, mesh_group_t* mesh_group,
    mesh_group_t::node_t* node, const mat4x4& transform)
{
    mat4x4 model;
    math::mat4x4_dup(model, node->transform);
    math::mat4x4_mul(model, transform);

    set_model_transform(model);
    for (uint32_t i = 0; i < node->meshes_size; i++) {
        auto* mesh = mesh_group->meshes[node->meshes[i]];
        mesh->texture = mesh_group->texture;
        draw_mesh(layer, mesh);
    }

    for (uint32_t i = 0; i < node->children_size; i++)
        draw_mesh_group_node(layer, mesh_group, &(node->children[i]), model);
}

void draw_mesh_group(layer_t layer, mesh_group_t* mesh_group)
{
    math::mat4x4 model;
    model_transform(model);

    draw_mesh_group_node(layer, mesh_group, &(mesh_group->root), model);
    // set_indice_buffer(mesh->ib);
    // set_vertex_buffer(mesh->vb);
    // set_texture(0, mesh->texture);
    // draw_element(layer, 0, mesh->ib->size);
}

} // namespace nx::gfx
