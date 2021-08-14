#ifndef NX_MISC_RINGBUFFER_H
#define NX_MISC_RINGBUFFER_H

#include <niu2x/utils.h>

namespace nx {

template <class Elem, size_t UserCapacity>
class ringbuffer {
public:
    static constexpr size_t Capacity = UserCapacity + 1;
    ringbuffer()
    : head_(0)
    , tail_(0)
    {
    }
    ~ringbuffer() { }

    ringbuffer(const ringbuffer&) = default;
    ringbuffer& operator=(const ringbuffer&) = default;

    status put_elem(const Elem& e) noexcept { return put(&e, 1, nullptr); }

    status put(const Elem* input, size_t isize, size_t* osize) noexcept
    {
        auto rooms = (Capacity - 1) - size();
        auto writen = min(isize, rooms);
        if (osize)
            *osize = writen;

        if (!writen)
            return again;

        if (tail_ >= head_) {
            auto n = min(Capacity - tail_, writen);
            memcpy(data_ + tail_, input, n * sizeof(Elem));
            input += n;
            writen -= n;
            tail_ = add(tail_, n);
        }

        if (writen) {
            memcpy(data_ + tail_, input, writen * sizeof(Elem));
            tail_ = add(tail_, writen);
        }

        return ok;
    }

    status get(Elem* output, size_t max_osize, size_t* osize) noexcept
    {

        auto my_size = size();
        if (!my_size)
            return eof;

        auto readn = min(max_osize, my_size);
        if (osize)
            *osize = readn;

        if (tail_ >= head_) {
            memcpy(output, data_ + head_, readn * sizeof(Elem));
        } else {
            memcpy(output, data_ + head_,
                min(Capacity - head_, readn) * sizeof(Elem));
            if (readn > Capacity - head_) {
                memcpy(output + Capacity - head_, data_,
                    readn - (Capacity - head_) * sizeof(Elem));
            }
        }

        head_ = add(head_, readn);
        return ok;
    }

    status get_elem(Elem* out) noexcept { return get(out, 1, nullptr); }

    size_t size() const noexcept { return minus(tail_, head_); }

    void pop(size_t n) noexcept { head_ = add(head_, min(n, size())); }
    void push(size_t n) noexcept
    {
        tail_ = add(tail_, min(n, Capacity - 1 - size()));
    }

    arrayref<Elem> continuous_elems() noexcept
    {
        if (tail_ >= head_) {
            return arrayref<Elem> { data_ + head_, tail_ - head_ };
        } else {
            return arrayref<Elem> { data_ + head_, Capacity - head_ };
        }
    }

    arrayref<Elem> continuous_slots() noexcept
    {
        if (tail_ >= head_) {
            if (head_) {
                return arrayref<Elem> { data_ + tail_, Capacity - tail_ };
            } else {
                return arrayref<Elem> { data_ + tail_, Capacity - 1 - tail_ };
            }
        } else {
            return arrayref<Elem> { data_ + tail_, head_ - tail_ - 1 };
        }
    }

    bool full() const noexcept { return size() == Capacity - 1; }

    bool empty() const noexcept { return !size(); }

    void normalize()
    {
        if (head_) {
            Elem temp[Capacity];
            memcpy(temp, data_ + head_, (Capacity - head_) * sizeof(Elem));
            memcpy(temp + Capacity - head_, data_, (head_) * sizeof(Elem));
            memcpy(data_, temp, Capacity * sizeof(Elem));

            size_t s = size();
            head_ = 0;
            tail_ = s;
        }
    }

private:
    Elem data_[Capacity];
    size_t head_;
    size_t tail_;

    static size_t minus(size_t a, size_t b) noexcept
    {
        if (a < b)
            return a + Capacity - b;
        return a - b;
    }

    static size_t add(size_t a, size_t b) noexcept
    {
        size_t sum = a + b;
        if (sum >= Capacity)
            sum -= Capacity;
        return sum;
    }
};
} // namespace nx

#endif