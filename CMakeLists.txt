cmake_minimum_required(VERSION 2.8.12)
project(roq-samples)
project(${PACKAGE_NAME} CXX)
# option(ROQ_SAMPLES_INSTALL "Install roq-samples header files?" ${_MASTER_PROJECT})
set(ROQ_SAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(ROQ_SAMPLES_INCLUDE_DIR "${ROQ_SAMPLES_DIR}" CACHE INTERNAL "")
add_library(roq_samples_common OBJECT
  ${ROQ_SAMPLES_DIR}/common/base_strategy.cpp
  ${ROQ_SAMPLES_DIR}/common/base_strategy.h
  ${ROQ_SAMPLES_DIR}/common/config.cpp
  ${ROQ_SAMPLES_DIR}/common/config.h
  ${ROQ_SAMPLES_DIR}/common/config_variables.cpp
  ${ROQ_SAMPLES_DIR}/common/config_variables.h
  ${ROQ_SAMPLES_DIR}/common/gateway.cpp
  ${ROQ_SAMPLES_DIR}/common/gateway.h
  ${ROQ_SAMPLES_DIR}/common/instrument.cpp
  ${ROQ_SAMPLES_DIR}/common/instrument.h
  ${ROQ_SAMPLES_DIR}/common/market_data.h
  ${ROQ_SAMPLES_DIR}/common/position.cpp
  ${ROQ_SAMPLES_DIR}/common/position.h
)
