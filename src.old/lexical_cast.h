#ifndef NX_LEXICAL_CAST_H
#define NX_LEXICAL_CAST_H

#include <boost/lexical_cast.hpp>

namespace nx {

template <class TO, class... FROM>
inline TO lexical_cast(FROM&&... from)
{
    return boost::lexical_cast<TO>(std::forward<FROM>(from)...);
}

} // namespace nx

#endif