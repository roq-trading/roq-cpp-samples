cmake_minimum_required(VERSION 3.0)

project(common)

add_library(common STATIC
  account.cpp
  config.cpp
  instrument.cpp
  market_data.cpp
  position.cpp
  strategy.cpp
)

target_include_directories(common PUBLIC
  ${ROQ_API_INCLUDE_DIR}
  ${ROQ_CLIENT_INCLUDE_DIR}
)
