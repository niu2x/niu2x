#ifndef NX_MISC_RINGBUFFER_H
#define NX_MISC_RINGBUFFER_H

#include <niu2x/misc/rw_status.h>

namespace nx::misc {

template<class Elem, size_t Capacity>
class ringbuffer {
public:
	ringbuffer():
	head_(0), tail_(0) {}
	~ringbuffer() {}

	ringbuffer(const ringbuffer &) = default;
	ringbuffer& operator=(const ringbuffer &) = default;

	rw_status put_elem(const Elem &e) noexcept {
		return put(&e, 1, nullptr);
	}

	rw_status put(const Elem *input, size_t isize, size_t *osize) noexcept {
		auto rooms = (Capacity-1) - size();
		auto writen = min(isize, rooms);
		if(osize) *osize = writen;

		if(!writen)
			return rw_status::again;

		if(tail_ >= head_) {
			auto n = min(Capacity - tail_, writen);
			memcpy(data_+tail_, input, n*sizeof(Elem));
			input += n;
			writen -= n;
			tail_ = add(tail_, n); 
		}

		if(writen){
			memcpy(data_+tail_, input, writen*sizeof(Elem));
			tail_ = add(tail_, writen); 
		}

		return rw_status::ok;
	}

	rw_status get(Elem *output, size_t max_osize, size_t *osize) noexcept {

		auto my_size = size();
		if(!my_size) 
			return rw_status::eof;

		auto readn = min(max_osize, my_size);
		if(osize) *osize = readn;

		if(tail_ >= head_) {
			memcpy(output, data_ + head_, readn*sizeof(Elem));
		}
		else{
			memcpy(output, data_ + head_,  min(Capacity-head_,  readn)*sizeof(Elem));
			if( readn > Capacity-head_){
				memcpy(output+Capacity-head_, data_,   readn - (Capacity-head_)*sizeof(Elem));
			}
		}

		head_ = add(head_, readn); 
		return rw_status::ok;


	}

	rw_status get_elem(Elem *out) noexcept {
		return get(out, 1, nullptr);
	}

	size_t size() const noexcept {
		return minus(tail_ , head_);
	}

private:

	Elem data_[Capacity];
	size_t head_;
	size_t tail_;

	static size_t minus(size_t a, size_t b) noexcept {
		if(a < b) return a + Capacity - b;
		return a - b;
	}

	static size_t add(size_t a, size_t b) noexcept {
		size_t sum = a + b;
		if(sum >= Capacity) sum-=Capacity;
		return sum;
	}
};


}

#endif