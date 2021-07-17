#ifndef NIU2X_LUA_ENGINE_H
#define NIU2X_LUA_ENGINE_H

#include <boost/noncopyable.hpp>

#include <niu2x/api.h>

namespace niu2x {

class API lua_engine : private boost::noncopyable{
public:
	lua_engine() ;
	~lua_engine() ;
};

}

#endif