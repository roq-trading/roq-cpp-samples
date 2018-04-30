find_path(FLATBUFFERS_INCLUDE_DIR flatbuffers/flatbuffers.h)
find_library(FLATBUFFERS_LIBS NAMES flatbuffers)

if (FLATBUFFERS_LIBS AND FLATBUFFERS_INCLUDE_DIR)
  set(Flatbuffers_FOUND TRUE)
  set(FLATBUFFERS_LIBS ${FLATBUFFERS_LIBS})
else ()
  set(Flatbuffers_FOUND FALSE)
endif ()

if (Flatbuffers_FOUND)
  if (NOT Flatbuffers_FIND_QUIETLY)
    message(STATUS "Found flatbuffers: ${FLATBUFFERS_LIBS}")
  endif ()
else ()
  if (Flatbuffers_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find flatbuffers.")
  endif ()
  message(STATUS "flatbuffers NOT found.")
endif ()

mark_as_advanced(
  FLATBUFFERS_LIBS
  FLATBUFFERS_INCLUDE_DIR
)
