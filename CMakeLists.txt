cmake_minimum_required(VERSION 3.5.1)
project(applicances_backend)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(
  include
  3rdparty/libproperty/include)

add_library(${PROJECT_NAME}
  src/appliances_backend/modbus_client.cpp
  src/appliances_backend/helios_kwl_modbus_client.cpp
  src/appliances_backend/appliance_base.cpp
  src/appliances_backend/interface_base.cpp
  src/appliances_backend/appliances/helios_kwl.cpp)

set(EXECUTABLE_SOURCES
  src/main.cpp)

add_executable(${PROJECT_NAME}-bin
  ${EXECUTABLE_SOURCES})

target_link_libraries(${PROJECT_NAME}-bin
  ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
  modbus
  mosquitto
  pthread)
