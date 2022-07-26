#include <niu2x/niu2x.h>
#include <niu2x/build.h>

struct params_t {
    struct {
        uint8_t version : 1;
    };
};

static void usage() { exit(1); }

static int params_parse(struct params_t* self, int argc, char* argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "dhv")) != -1) {
        switch (opt) {
            case 'v': {
                self->version = 1;
                break;
            }
            default: {
                usage();
            }
        }
    }
    return optind;
}

int main(int argc, char* argv[])
{
    struct params_t params;
    params_parse(&params, argc, argv);
    if (params.version) {
        printf("features: \n");
#if defined(OpenSSL_FOUND)
        printf("\t openssl\n");
#endif
#if defined(CURL_FOUND)
        printf("\t curl\n");
#endif
#if defined(libuv_FOUND)
        printf("\t libuv\n");
#endif
#if defined(ZLIB_FOUND)
        printf("\t ZLIB\n");
#endif
    }

    return 0;
}
