### TODO:
- async io support
	- libuv use custom allocator
- stl use custom allocator
- tolua use custom allocator


### Design Rules:
- c/c++ implement core functions that lua can't implement
- lua implement all others except optimization

### products
- niu2x: c++ library for myself
- niu2x_lua: lua wrapper for niu2x
- nx: lua standalone with niu2x support, used to implement some tools