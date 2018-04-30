find_path(UCL_INCLUDE_DIR ucl.h)
find_library(UCL_LIBS NAMES ucl)

if (UCL_LIBS AND UCL_INCLUDE_DIR)
  set(Ucl_FOUND TRUE)
  set(UCL_LIBS ${UCL_LIBS})
else ()
  set(Ucl_FOUND FALSE)
endif ()

if (Ucl_FOUND)
  if (NOT Ucl_FIND_QUIETLY)
    message(STATUS "Found ucl: ${UCL_LIBS}")
  endif ()
else ()
  if (Ucl_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find ucl.")
  endif ()
  message(STATUS "ucl NOT found.")
endif ()

mark_as_advanced(
  UCL_LIBS
  UCL_INCLUDE_DIR
)
