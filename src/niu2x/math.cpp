#include <niu2x/linmath.h>
#include <niu2x/log.h>

namespace nx::math {

namespace {
struct init_t {
    init_t() { mat4x4_identity(identity_mat4x4); }
};
} // namespace

static init_t init;

mat4x4 identity_mat4x4;

} // namespace nx::math