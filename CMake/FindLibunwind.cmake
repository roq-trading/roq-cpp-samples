find_path(LIBUNWIND_INCLUDE_DIR libunwind.h)
find_library(LIBUNWIND_LIBS NAMES unwind)

if (LIBUNWIND_LIBS AND LIBUNWIND_INCLUDE_DIR)
  set(Libunwind_FOUND TRUE)
  set(LIBUNWIND_LIBS ${LIBUNWIND_LIBS})
else ()
  set(Libunwind_FOUND FALSE)
endif ()

if (Libunwind_FOUND)
  if (NOT Libunwind_FIND_QUIETLY)
    message(STATUS "Found libunwind: ${LIBUNWIND_LIBS}")
  endif ()
else ()
  if (Libunwind_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find libunwind.")
  endif ()
  message(STATUS "libunwind NOT found.")
endif ()

mark_as_advanced(
  LIBUNWIND_LIBS
  LIBUNWIND_INCLUDE_DIR
)
