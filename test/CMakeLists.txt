set(TARGET_NAME "${PROJECT_NAME}-test")

add_executable("${TARGET_NAME}" main.cpp)

# target

target_link_libraries("${TARGET_NAME}" roq-client::roq-client gtest_main)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_17)

# run

add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")
