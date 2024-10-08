cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")
project(SynapTCP VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} LANGUAGES CXX)

include(FindPkgConfig)
include(FetchContent)
FetchContent_Declare (
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

# Set the default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Check the target architecture
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
  set(TARGET_ARCH arm64)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  set(TARGET_ARCH amd64)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64")
  set(TARGET_ARCH amd64)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armhf")
  set(TARGET_ARCH armhf)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "armv7l")
  set(TARGET_ARCH armhf)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i386")
  set(TARGET_ARCH intel32)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
  set(TARGET_ARCH arm64)
else()
  message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# Verbose compile option
option(VERBOSE "Enable verbose compile" OFF)
if(VERBOSE)
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# Find and Check Library
find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)

# Specify the source files
set(SOURCE_FILES
    src/layer-ssl.cpp
    src/socket.cpp
    src/synapsock.cpp
    src/tcp-client.cpp
    src/tcp-server.cpp
)

# Create a library from common code
add_library(${PROJECT_NAME}-lib SHARED
  ${SOURCE_FILES}
)

# Set library output name
set_target_properties(${PROJECT_NAME}-lib PROPERTIES
  OUTPUT_NAME ${PROJECT_NAME}
)

# Skip creating symbolic links
set_target_properties(${PROJECT_NAME}-lib PROPERTIES
  SKIP_BUILD_RPATH 1
)

# Create the executable
add_executable(${PROJECT_NAME}-client examples/data-formating.cpp examples/client.cpp)
add_executable(${PROJECT_NAME}-server examples/data-formating.cpp examples/server.cpp)

# Create Unit Test executable
add_executable(${PROJECT_NAME}-test test/test-simple.cpp test/test-framed-data.cpp test/test-ssl-simple.cpp)

# Include directories
target_include_directories(${PROJECT_NAME}-lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/DataFrame/include>
)

include_directories(${PROJECT_NAME}-test PUBLIC ${GTEST_INCLUDE_DIRS})

# Ensure DataFrame is built before the executable
add_dependencies(${PROJECT_NAME}-client DataFrame-lib)
add_dependencies(${PROJECT_NAME}-server DataFrame-lib)
add_dependencies(${PROJECT_NAME}-test DataFrame-lib)

# Link the executable with the library
target_link_libraries(${PROJECT_NAME}-client PRIVATE ${PROJECT_NAME}-lib DataFrame-lib)
target_link_libraries(${PROJECT_NAME}-client PUBLIC -lpthread -lssl)
target_link_libraries(${PROJECT_NAME}-server PRIVATE ${PROJECT_NAME}-lib DataFrame-lib)
target_link_libraries(${PROJECT_NAME}-server PUBLIC -lpthread -lssl)

add_subdirectory(external/DataFrame EXCLUDE_FROM_ALL)
set(USE_EXE_FUNC ON)
set(USE_POST_EXE_FUNC ON)
add_definitions(-D__USE_EXE_FUNC)
add_definitions(-D__USE_POST_FUNC)
add_definitions(-D__STCP_SSL__)

# Link Unit Test executable with the library
target_link_libraries(${PROJECT_NAME}-test PRIVATE ${PROJECT_NAME}-lib DataFrame-lib)
target_link_libraries(${PROJECT_NAME}-test PUBLIC ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} -lpthread -lssl)

# Set compiler and linker flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -O3")
set(CMAKE_USE_RELATIVE_PATHS OFF)

# Add test
add_test(NAME example_test COMMAND ${PROJECT_NAME}-test)

# Setup for package installer
install(TARGETS ${PROJECT_NAME}-lib
  EXPORT "${PROJECT_NAME}Targets"
  DESTINATION "/usr/lib/"
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Setup for package generator
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Packing)
