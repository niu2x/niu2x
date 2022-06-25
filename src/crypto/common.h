#ifndef NX_SRC_CRYPTO_COMMON_H
#define NX_SRC_CRYPTO_COMMON_H

#define DEFINE_CRYPTO_STR_FUNC(name)                                           \
    std::string name(const void* input, int size)                              \
    {                                                                          \
        int buffer_size = name##_size(input, size);                            \
        uint8_t* buffer = new uint8_t[buffer_size];                            \
        NX_THROW_COND_MSG(!buffer, "alloc buffer failed");                     \
        int out_size = name(buffer, input, size);                              \
        std::string result(buffer, buffer + out_size);                         \
        delete[] buffer;                                                       \
        return result;                                                         \
    }

#endif