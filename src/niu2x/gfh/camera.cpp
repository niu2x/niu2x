#include <niu2x/gfh.h>

namespace nx::gfh {

camera_t::camera_t() { math::mat4x4_identity(transform_); }

camera_t::~camera_t() { }

void camera_t::look_at(
    math::vec3 const eye, math::vec3 const center, math::vec3 const up)
{
    math::mat4x4_look_at(transform_, eye, center, up);
}

void camera_t::pitch(double delta) { math::mat4x4_rotate_x(transform_, delta); }
void camera_t::yaw(double delta) { math::mat4x4_rotate_y(transform_, delta); }
void camera_t::roll(double delta) { math::mat4x4_rotate_z(transform_, delta); }

void camera_t::move(double z) { math::mat4x4_translate(transform_, 0, 0, z); }

} // namespace nx::gfh