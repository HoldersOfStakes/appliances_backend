cmake_minimum_required(VERSION 3.5.1)
project(applicances_backend)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(
  include)

add_library(${PROJECT_NAME}
  src/appliances_backend/utilities/modbus_client.cpp
  src/appliances_backend/utilities/mqtt_client.cpp
  src/appliances_backend/appliances/helios_kwl/helios_kwl_modbus_client.cpp
  src/appliances_backend/manageable_base.cpp
  src/appliances_backend/variable_managing_base.cpp
  src/appliances_backend/appliance_base.cpp
  src/appliances_backend/interface_base.cpp
  src/appliances_backend/appliances/helios_kwl/helios_kwl.cpp
  src/appliances_backend/interfaces/homebridge/homebridge_mqtt.cpp
  src/appliances_backend/appliances_manager.cpp
  src/appliances_backend/interfaces_manager.cpp
  src/appliances_backend/manager_base.cpp
  src/appliances_backend/accessory.cpp
  src/appliances_backend/service.cpp
  src/appliances_backend/characteristic.cpp
  src/appliances_backend/mapper.cpp
  src/appliances_backend/accessories_manager.cpp
  src/appliances_backend/backend.cpp)

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
