#ifndef NX_FREELIST_H
#define NX_FREELIST_H

#include <limits>

#include "utils.h"

namespace nx {

using freelist_rid = uint64_t;

template <class T, freelist_rid N>
class freelist {
public:
    using rid = freelist_rid;
    static constexpr rid nil = 0;

    template <class TT>
    union slot {
        TT data;
        rid next;
        slot() { }
        ~slot() { }
    };

    freelist()
    {
        next_ = 1;
        slot<T>* ptr = slots_;
        for (rid i = 1; i < N; i++, ptr++) {
            ptr->next = i + 1;
        }
        ptr->next = nil;
        size_ = 0;
    }

    freelist(const freelist&) = default;
    freelist& operator=(const freelist&) = default;

    rid alloc()
    {
        if (next_ == nil)
            return nil;
        auto result = next_;
        next_ = slots_[result - 1].next;
        ++size_;
        return result;
    }

    void free(rid p_rid)
    {
        if (p_rid == nil)
            return;

        --size_;
        slots_[p_rid - 1].next = next_;
        next_ = p_rid;
    }

    T* get(rid p_rid)
    {
        NX_ASSERT(p_rid != nil, "p_rid is rid::nil");
        return &(slots_[p_rid - 1].data);
    }

private:
    slot<T> slots_[N];
    rid next_;
    rid size_;
};

} // namespace nx

#endif