#include <niu2x/gfx_utils.h>

namespace nx::gfx_utils {

gfx::mesh_t* mesh_create_plane(double width, double height)
{
    auto* mesh = gfx::mesh_create();
    struct vertex_t {
        float x, y, z;
        float nx, ny, nz;
        float uvx, uvy, uvz;
    } vertices[4] = {
        { (float)(-width * 0.5), (float)(-height * 0.5), 0, 0, 0, 1, 0, 1, 0 },
        { (float)(width * 0.5), (float)(-height * 0.5), 0, 0, 0, 1, 1, 1, 0 },
        { (float)(width * 0.5), (float)(height * 0.5), 0, 0, 0, 1, 1, 0, 0 },
        { (float)(-width * 0.5), (float)(height * 0.5), 0, 0, 0, 1, 0, 0, 0 },
    };
    auto vertex_layout = gfx::vertex_layout(gfx::vertex_attr_t::position,
        gfx::vertex_attr_t::normal, gfx::vertex_attr_t::uv);
    uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };
    mesh->vb = gfx::vertex_buffer_create(vertex_layout, 4, &vertices);
    mesh->ib = gfx::indice_buffer_create(2 * 3, indices);
    return mesh;
}

} // namespace nx::gfx_utils
