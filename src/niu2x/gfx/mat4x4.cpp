#include "gfx.h"

#include <string.h>

namespace nx::gfx {

template <int N>
static void multiply(const double A[][N], const double B[][N], double C[][N])
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void mat4x4_mul(
    struct mat4x4_t* output, const struct mat4x4_t& a, const struct mat4x4_t& b)
{
    using temp = double[4];
    multiply<4>((temp*)(a.data), (temp*)(b.data), (temp*)(output->data));
}

void mat4x4_add(
    struct mat4x4_t* output, const struct mat4x4_t& a, const struct mat4x4_t& b)
{
    for (int i = 0; i < 16; i++)
        output->data[i] = a.data[i] + b.data[i];
}

void mat4x4_dump(const struct mat4x4_t& a)
{
    printf("mat4x4 %p: \n", &a);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", a.data[i * 4 + j]);
        }
        printf("\n");
    }
}

void mat4x4_set(struct mat4x4_t* output, const double* data)
{
    memcpy(output->data, data, sizeof(double) << 4);
}

} // namespace nx::gfx