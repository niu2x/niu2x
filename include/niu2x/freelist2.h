#ifndef NX_FREELIST2_H
#define NX_FREELIST2_H

#include <limits>

#include <niu2x/utils.h>

namespace nx {

using freelist_rid_t = uint64_t;

template <class T, freelist_rid_t N>
class freelist_t {
public:
    using rid_t = freelist_rid_t;
    static constexpr rid_t nil = 0;

    template <class TT>
    union slot {
        TT data;
        rid_t next;
        slot() { }
        ~slot() { }
    };

    freelist_t()
    {
        next_ = 1;
        slot<T>* ptr = slots_;
        for (rid_t i = 1; i < N; i++, ptr++) {
            ptr->next = i + 1;
        }
        ptr->next = nil;
        size_ = 0;
    }

    freelist_t(const freelist_t&) = default;
    freelist_t& operator=(const freelist_t&) = default;

    rid_t alloc()
    {
        if (next_ == nil)
            return nil;
        auto result = next_;
        next_ = slots_[result - 1].next;
        ++size_;
        return result;
    }

    void free(rid_t p_rid)
    {
        if (p_rid == nil)
            return;

        --size_;
        slots_[p_rid - 1].next = next_;
        next_ = p_rid;
    }

    T* get(rid_t p_rid) { return &(slots_[p_rid - 1].data); }

private:
    slot<T> slots_[N];
    rid_t next_;
    rid_t size_;
};

} // namespace nx

#endif