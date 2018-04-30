find_path(CROSSGUID_INCLUDE_DIR crossguid/guid.hpp)
find_library(CROSSGUID_LIBS NAMES crossguid uuid)  # OSX: '-framework CoreFoundation'

if (CROSSGUID_LIBS AND CROSSGUID_INCLUDE_DIR)
  set(Crossguid_FOUND TRUE)
  set(CROSSGUID_LIBS ${CROSSGUID_LIBS})
else ()
  set(Crossguid_FOUND FALSE)
endif ()

if (Crossguid_FOUND)
  if (NOT Crossguid_FIND_QUIETLY)
    message(STATUS "Found crossguid: ${CROSSGUID_LIBS}")
  endif ()
else ()
  if (Crossguid_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find crossguid.")
  endif ()
  message(STATUS "crossguid NOT found.")
endif ()

mark_as_advanced(
  CROSSGUID_LIBS
  CROSSGUID_INCLUDE_DIR
)
