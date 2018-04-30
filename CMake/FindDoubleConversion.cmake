find_path(DOUBLE_CONVERSION_INCLUDE_DIR double-conversion/double-conversion.h)
find_library(DOUBLE_CONVERSION_LIBS NAMES double-conversion)

if (DOUBLE_CONVERSION_LIBS AND DOUBLE_CONVERSION_INCLUDE_DIR)
  set(DoubleConversion_FOUND TRUE)
  set(DOUBLE_CONVERSION_LIBS ${DOUBLE_CONVERSION_LIBS})
else ()
  set(DoubleConversion_FOUND FALSE)
endif ()

if (DoubleConversion_FOUND)
  if (NOT DoubleConversion_FIND_QUIETLY)
    message(STATUS "Found double-conversion: ${DOUBLE_CONVERSION_LIBS}")
  endif ()
else ()
  if (DoubleConversion_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find double-conversion.")
  endif ()
  message(STATUS "double-conversion NOT found.")
endif ()

mark_as_advanced(
  DOUBLE_CONVERSION_LIBS
  DOUBLE_CONVERSION_INCLUDE_DIR
)
