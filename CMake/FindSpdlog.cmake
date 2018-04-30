find_path(SPDLOG_INCLUDE_DIR spdlog/spdlog.h)

if (SPDLOG_INCLUDE_DIR)
  set(Spdlog_FOUND TRUE)
else ()
  set(Spdlog_FOUND FALSE)
endif ()

if (Spdlog_FOUND)
  if (NOT Spdlog_FIND_QUIETLY)
    message(STATUS "Found spdlog")
  endif ()
else ()
  if (Spdlog_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find spdlog.")
  endif ()
  message(STATUS "spdlog NOT found.")
endif ()

mark_as_advanced(
  SPDLOG_INCLUDE_DIR
)
