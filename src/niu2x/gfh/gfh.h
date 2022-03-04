#include <niu2x/gfh.h>
#include "niu2x/obj_pool.h"

namespace nx::gfh {

extern NX_OBJ_POOL_DECLARE2(game_object);
extern NX_OBJ_POOL_DECLARE2(transform);


#define component_retain(self)  ++ (self)->ref;
#define component_release(self)  \
if(--(self)->ref == 0) { \
        destroy((self)); \
    } 

#define component_setup(self) (self)->ref = 1;
#define component_cleanup(self) unused(self);

}