# Modules
- [log](#log)
- [pipe (crypto/digest/base64/zlib)](#pipe)
- gfx (TODO)

# Examples
please goto test/\*.cpp

## log
````
NX_LOG_T(format, ...);
NX_LOG_D(format, ...);
NX_LOG_W(format, ...);
NX_LOG_E(format, ...);
NX_LOG_F(format, ...);
````
## pipe
- pipe style

#### read stdin -> compress -> write to stdout.
````
nx::pipe::filter::zlib_t zlib;
nx::pipe::source_t(std::cin) | zlib | nx::pipe::sink_t(std::cout);
````

#### read stdin -> remove all '\n' -> unbase64 -> write to stdout
````
nx::pipe::filter::unbase64_t unbase64;
nx::pipe::filter::cut_t cut('\n');
nx::pipe::source_t(std::cin) | cut | unbase64 | nx::pipe::sink_t(std::cout);

````
- supported filters
    - lower/upper
    - hex/unhex
    - cut
    - base64/unbase64
    - digest (md5/sha1/sha256/etc ...) base on openssl
    - encrypt/decrypt(aes-128-cbc/etc ...) base on openssl 

