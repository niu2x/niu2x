#include <niu2x/utils.h>
#include <niu2x/crypto.h>

void hidden_symbol_function() { }
int hidden_symbol_variable = 0;

void unused_test(int a, int b)
{
    NX_UNUSED(a, b);
    auto k = nx::crypto::md5_size;
    NX_UNUSED(k);
}
