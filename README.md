# Modules
- [log](##log)
- [io](##io)
- gfx (TODO)

## log
````
NX_LOG_T(format, ...);
NX_LOG_D(format, ...);
NX_LOG_W(format, ...);
NX_LOG_E(format, ...);
NX_LOG_F(format, ...);
````
## io
- pipe style
````
//read stdin -> compress -> write to stdout.
io::filter::zlib zlib;
io::source(std::cin) | zlib | io::sink(std::cout);

//read stdin -> remove all '\n' -> unbase64 -> write to stdout
nx::io::filter::unbase64 unbase64;
nx::io::filter::cut cut('\n');
nx::io::source(std::cin) | cut | unbase64 | nx::io::sink(std::cout);

````
- supported filters
    - lower/upper
    - hex/unhex
    - cut
    - base64/unbase64
    - digest (md5/sha1/sha256/etc ...) base on openssl
    - encrypt/decrypt(aes-128-cbc/etc ...) base on openssl 

