#ifndef _DBG_H_
#define _DBG_H_

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

#define release_mem(TARGET, FREE_FUNCTION) \
    if (TARGET) { \
        FREE_FUNCTION(TARGET);\
    }

#define release_mem_arr(TARGET, NELEM, FREE_FUNCTION) \
    if (TARGET) { \
        int z; \
        for (z = 0; z < NELEM; z++) { \
            FREE_FUNCTION(TARGET[z]);\
        } \
        FREE_FUNCTION(TARGET); \
    }

#define release_mem_2darr(TARGET, ROWS, COLS, FREE_FUNCTION) \
    if (TARGET) { \
        int a; \
        int b; \
        for (a = 0; a < d->cols; a++) { \
            for (b = 0; b < d->rows; b++) { \
                FREE_FUNCTION(TARGET[a][b]); \
            } \
            FREE_FUNCTION(TARGET[a]); \
        } \
        FREE_FUNCTION(TARGET); \
    }

#endif
