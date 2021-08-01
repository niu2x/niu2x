#ifndef GXM_FREELIST_H
#define GXM_FREELIST_H

#include <limits>

#include <niu2x/utils.h>

namespace nx::misc {

using freelist_uint_t = uint16_t;

struct rid {
    using id_t = freelist_uint_t;
    freelist_uint_t id;

    static const rid nil;
};

inline bool operator==(const rid& a, const rid& b) noexcept
{
    return a.id == b.id;
}
inline bool operator!=(const rid& a, const rid& b) noexcept
{
    return a.id != b.id;
}

template <class T, freelist_uint_t N>
class freelist {
public:
    template <class TT>
    union slot {
        TT data;
        freelist_uint_t next;

        slot() { }
        ~slot() { }
    };

    freelist()
    {
        next_ = 0;

        slot<T>* ptr = slots_;
        for (freelist_uint_t i = 0; i < N - 1; i++, ptr++) {
            ptr->next = i + 1;
        }
        ptr->next = rid::nil.id;
    }

    rid alloc()
    {
        if (next_ == rid::nil.id)
            return rid::nil;
        auto result = next_;

        next_ = slots_[result].next;
        return { result };
    }

    void free(rid p_rid)
    {
        NX_ASSERT(p_rid != rid::nil, "p_rid is rid::nil");
        slots_[p_rid.id].next = next_;

        next_ = p_rid.id;
    }

    T* get(rid p_rid)
    {
        NX_ASSERT(p_rid != rid::nil, "p_rid is rid::nil");
        return &(slots_[p_rid.id].data);
    }

private:
    slot<T> slots_[N];
    freelist_uint_t next_;
};

} // namespace nx::misc

#endif