find_path(GFLAGS_INCLUDE_DIR gflags/gflags.h)
find_library(GFLAGS_LIBS NAMES gflags)

if (GFLAGS_LIBS AND GFLAGS_INCLUDE_DIR)
  set(Gflags_FOUND TRUE)
  set(GFLAGS_LIBS ${GFLAGS_LIBS})
else ()
  set(Gflags_FOUND FALSE)
endif ()

if (Gflags_FOUND)
  if (NOT Gflags_FIND_QUIETLY)
    message(STATUS "Found gflags: ${GFLAGS_LIBS}")
  endif ()
else ()
  if (Gflags_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find gflags.")
  endif ()
  message(STATUS "gflags NOT found.")
endif ()

mark_as_advanced(
  GFLAGS_LIBS
  GFLAGS_INCLUDE_DIR
)
