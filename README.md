# Modules
- [log](#log)
- [pipe](#pipe (crypto/digest/base64/hex))
- gfx (TODO)

# Examples
please goto test/*.cpp

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
````
//read stdin -> compress -> write to stdout.
nx::pipe::filter::zlib zlib;
nx::pipe::source(std::cin) | zlib | nx::pipe::sink(std::cout);

//read stdin -> remove all '\n' -> unbase64 -> write to stdout
nx::pipe::filter::unbase64 unbase64;
nx::pipe::filter::cut cut('\n');
nx::pipe::source(std::cin) | cut | unbase64 | nx::pipe::sink(std::cout);

````
- supported filters
    - lower/upper
    - hex/unhex
    - cut
    - base64/unbase64
    - digest (md5/sha1/sha256/etc ...) base on openssl
    - encrypt/decrypt(aes-128-cbc/etc ...) base on openssl 

