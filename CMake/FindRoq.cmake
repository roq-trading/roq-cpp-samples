find_path(ROQ_INCLUDE_DIR roq/api.h)
find_library(ROQ_LIBS NAMES roq)

if (ROQ_LIBS AND ROQ_INCLUDE_DIR)
  set(Roq_FOUND TRUE)
  set(ROQ_LIBS ${ROQ_LIBS})
else ()
  set(Roq_FOUND FALSE)
endif ()

if (Roq_FOUND)
  if (NOT Roq_FIND_QUIETLY)
    message(STATUS "Found roq: ${ROQ_LIBS}")
  endif ()
else ()
  if (Roq_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find roq.")
  endif ()
  message(STATUS "roq NOT found.")
endif ()

mark_as_advanced(
  ROQ_LIBS
  ROQ_INCLUDE_DIR
)
