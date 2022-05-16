set(TARGET_NAME "${PROJECT_NAME}-zeromq-zmq")

set(SOURCES context.cpp socket.cpp)

add_library("${TARGET_NAME}" STATIC ${SOURCES})

target_link_libraries("${TARGET_NAME}" fmt::fmt-header-only zmq)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_20)
