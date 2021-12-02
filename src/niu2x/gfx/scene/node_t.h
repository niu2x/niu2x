#ifndef NX_GFX_SCENE_H
#define NX_GFX_SCENE_H

#include <cstdint>

namespace nx::gfx::scene {

class node_t {
public:
    using id_t = uint64_t;
    node_t();
    ~node_t();

private:
    id_t id_;
    static id_t next_id_;
};

}; // namespace nx::gfx::scene

#endif