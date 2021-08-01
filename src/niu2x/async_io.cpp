#include "async_io.h"

#include <uv.h>

namespace nx::async_io {

namespace {

class uv_event_loop: public event_loop {
public:
	uv_event_loop(){
	uv_loop_init(&loop_);
		
	}
	virtual ~uv_event_loop(){
	uv_loop_close(&loop_);


	}

	virtual void run(type p_type) {
		switch(p_type) {
			case type::wait: {
	            uv_run(&loop_, UV_RUN_DEFAULT);
				break;
			}
			case type::wait_once: {
	            uv_run(&loop_, UV_RUN_ONCE);
				break;
			}
			case type::no_wait: {
	            uv_run(&loop_, UV_RUN_NOWAIT);
				break;
			}
		}
	}

private:
	uv_loop_t loop_;
};

}

}


namespace nx::async_io {


event_loop::event_loop() {
}
event_loop::~event_loop() {
}

std::unique_ptr<event_loop> event_loop::create() {
	return std::make_unique<uv_event_loop>();
}



}