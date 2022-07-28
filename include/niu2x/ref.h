#ifndef NX_REF_H
#define NX_REF_H

#include <atomic>
#include <niu2x/api.h>
#include <niu2x/utils.h>

namespace nx {

struct ref_t {
    std::atomic<int64_t> refcount;
};

static inline void ref_init(ref_t* ref)
{
    new (&(ref->refcount)) std::atomic<int64_t> { 1 };
}

static inline ref_t* ref_get(ref_t* ref)
{
    ref->refcount++;
    return ref;
}
static inline void ref_put(ref_t* ref, void (*release)(ref_t*))
{
    if (ref->refcount.fetch_sub(1) == 1) {
        call_destructor(&(ref->refcount));
        release(ref);
    }
}

} // namespace nx

#endif