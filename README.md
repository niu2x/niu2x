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
- lnx: lua wrapper for niu2x
- nxlua: lua standalone with niu2x support