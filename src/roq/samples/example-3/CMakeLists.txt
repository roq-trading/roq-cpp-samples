set(TARGET_NAME "${PROJECT_NAME}-example-3")

add_subdirectory(flags)

add_executable(
  "${TARGET_NAME}"
  application.cpp
  config.cpp
  ema.cpp
  flags.cpp
  instrument.cpp
  model.cpp
  strategy.cpp
  main.cpp)

target_link_libraries("${TARGET_NAME}" PRIVATE ${TARGET_NAME}-flags roq-client::roq-client
                                               roq-logging::roq-logging absl::flags fmt::fmt)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_17)

install(TARGETS ${TARGET_NAME})
