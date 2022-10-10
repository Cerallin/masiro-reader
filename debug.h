#ifndef DEBUG_H
#define DEBUG_H

#include <assert.h>

#ifndef NDEBUG

#define debug_assert(...) assert(__VA_ARGS__)
#define debug_static_assert(...) static_assert(__VA_ARGS__)

#define debug_is_initialized(ptr) debug_assert(ptr != nullptr)

#else

#define debug_assert()
#define debug_static_assert()

#endif /* NDEBUG */

#endif /* DEBUG_H */