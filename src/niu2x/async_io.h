#ifndef ASYNC_IO_H
#define ASYNC_IO_H

#include <niu2x/utils.h>

namespace nx::async_io {

class API event_loop: private noncopyable {
public:
	enum class type {
		wait,
		wait_once,
		no_wait,
	};

	static  std::unique_ptr<event_loop> create();
	
	event_loop();
	virtual ~event_loop();

	virtual void run(type p_type) = 0;
	void run() {
		run(type::wait);
	}
private:

};

}

namespace nx::aio {

using namespace nx::async_io;

}

#endif