#include "freelist.h"

namespace nx::misc {

const rid rid::nil { std::numeric_limits<freelist_uint_t>::max() };

} // namespace nx::misc