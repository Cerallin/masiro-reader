#ifndef DEBUG_H
#define DEBUG_H

#include "config.h"

#include <assert.h>
#include <exception>
#include <inttypes.h>
#include <iostream>
#include <system_error>


#define assert_is_initialized(ptr) assert(ptr != nullptr)

#ifndef NDEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#define handle_exception(e) __handle(__PRETTY_FUNCTION__, e)

#else
#define debug(...)
#define handle_exception(e)

#endif /* ifdef NDEBUG */

static inline void __handle(const char *hint, const std::exception &e) {
    std::cerr << hint << ": " << e.what() << std::endl;
}

static inline void __handle(const char *hint, const std::system_error &e) {
    std::cerr << hint << "(" << e.code() << "): " << e.what() << std::endl;
}

#endif /* DEBUG_H */
