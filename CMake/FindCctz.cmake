find_path(CCTZ_INCLUDE_DIR cctz/civil_time.h)
find_library(CCTZ_LIBS NAMES cctz)

if (CCTZ_LIBS AND CCTZ_INCLUDE_DIR)
  set(Cctz_FOUND TRUE)
  set(CCTZ_LIBS ${CCTZ_LIBS})
else ()
  set(Cctz_FOUND FALSE)
endif ()

if (Cctz_FOUND)
  if (NOT Cctz_FIND_QUIETLY)
    message(STATUS "Found cctz: ${CCTZ_LIBS}")
  endif ()
else ()
  if (Cctz_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find cctz.")
  endif ()
  message(STATUS "cctz NOT found.")
endif ()

mark_as_advanced(
  CCTZ_LIBS
  CCTZ_INCLUDE_DIR
)
