set(TARGET_NAME ${PROJECT_NAME}-algo-strategies)

set(SOURCES base.cpp factory.cpp order_manager.cpp spread.cpp)

add_library(${TARGET_NAME} OBJECT ${SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE roq-api::roq-api absl::flags absl::flat_hash_map
                                             absl::flat_hash_set)
