#ifndef __DBG__
#define __DBG__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
  #define debug(M, ...)
#else
  #define debug(M, ...) \
      fprintf(stderr, \
          "[DEBUG] %s:%d: " M "\n", \
          __func__, \
          __LINE__, \
          ##__VA_ARGS__ \
      )
#endif

#define log_err(M, ...) \
    fprintf(stderr,\
        "[ERROR] (%s:%d) " M "\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__\
    )

#define log_warn(M, ...) \
    fprintf(stderr,\
        "[WARN] (%s:%d) " M "\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__\
    )

#define log_info(M, ...) \
    fprintf(stderr, \
        "[INFO] (%s:%d) " M "\n", \
        __FILE__, \
        __LINE__, \
        ##__VA_ARGS__\
    )

#define check(A, M, ...) \
    if (!(A)) { \
        log_err(M, ##__VA_ARGS__); goto error; \
    }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) \
    if (!(A)) { \
        debug(M, ##__VA_ARGS__); goto error; \
    }

#define silent_check(A) \
    if (!(A)) { \
        goto error; \
    }

#endif
