set(TARGET_NAME "${PROJECT_NAME}-benchmark")

add_executable("${TARGET_NAME}" main.cpp)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_17)

target_link_libraries("${TARGET_NAME}" roq-client::roq-client benchmark::benchmark)
