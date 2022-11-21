#ifndef DEBUG_H
#define DEBUG_H

#include "config.h"

#include <inttypes.h>
#include <assert.h>
#include <stdio.h>

#define assert_is_initialized(ptr) assert(ptr != nullptr)

#ifndef NDEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)

#else
#define debug(...)

#endif /* ifdef NDEBUG */

#endif /* DEBUG_H */
