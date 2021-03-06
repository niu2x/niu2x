#ifndef NX_SRC_CRYPTO_COMMON_H
#define NX_SRC_CRYPTO_COMMON_H

#define DEFINE_CRYPTO_STR_FUNC(name)                                           \
    std::string name(const void* input, int size)                              \
    {                                                                          \
        int buffer_size = name##_size(input, size);                            \
        uint8_t* buffer = new uint8_t[buffer_size];                            \
        int out_size = name(buffer, input, size);                              \
        std::string result(buffer, buffer + out_size);                         \
        delete[] buffer;                                                       \
        return result;                                                         \
    }

#define DEFINE_CRYPTO_STR2_FUNC(name)                                          \
    std::string name(const std::string& input)                                 \
    {                                                                          \
        return name(input.c_str(), input.size());                              \
    }

#endif