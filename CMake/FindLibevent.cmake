find_path(LIBEVENT_INCLUDE_DIR event2/event.h)
find_library(LIBEVENT_LIBS NAMES event)

if (LIBEVENT_LIBS AND LIBEVENT_INCLUDE_DIR)
  set(Libevent_FOUND TRUE)
  set(LIBEVENT_LIBS ${LIBEVENT_LIBS})
else ()
  set(Libevent_FOUND FALSE)
endif ()

if (Libevent_FOUND)
  if (NOT Libevent_FIND_QUIETLY)
    message(STATUS "Found libevent: ${LIBEVENT_LIBS}")
  endif ()
else ()
  if (Libevent_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find libevent.")
  endif ()
  message(STATUS "libevent NOT found.")
endif ()

mark_as_advanced(
  LIBEVENT_LIBS
  LIBEVENT_INCLUDE_DIR
)
