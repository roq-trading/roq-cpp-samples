set(TARGET_NAME "${PROJECT_NAME}-algo-strategies")

set(SOURCES base.cpp factory.cpp order_manager.cpp spread.cpp)

add_library("${TARGET_NAME}" STATIC ${SOURCES})

target_link_libraries("${TARGET_NAME}" absl::flags absl::flat_hash_map absl::flat_hash_set)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_20)
