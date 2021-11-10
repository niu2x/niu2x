#include <sstream>

#include <niu2x/gfx.h>

namespace gfx = nx::gfx;

int main(int argc, char* argv[])
{
    gfx::mat4x4_t m;
    gfx::mat4x4_t n;

    double d[] = { 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4 };

    gfx::mat4x4_set(&m, d);
    gfx::mat4x4_dump(m);
    gfx::mat4x4_add(&n, m, m);
    gfx::mat4x4_dump(n);
    gfx::mat4x4_mul(&n, m, m);
    gfx::mat4x4_dump(n);
    return 0;
}