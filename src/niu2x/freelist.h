#ifndef GXM_FREELIST_H
#define GXM_FREELIST_H

#include <limits>

#include <niu2x/utils.h>

namespace nx {

template <class T, rid N>
class freelist {
public:
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
    }

    rid alloc()
    {
        if (next_ == nil)
            return nil;
        auto result = next_;
        next_ = slots_[result - 1].next;
        return result;
    }

    void free(rid p_rid)
    {
        if (p_rid == nil)
            return;

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
};

} // namespace nx

#endif