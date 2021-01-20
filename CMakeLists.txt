cmake_minimum_required(VERSION 2.6)

project(visutiles LANGUAGES CXX)

set(visutiles_VERSION_MAJOR 0)
set(visutiles_VERSION_MINOR 1)
set(visutiles_PROG_NAME "VisuTiles")
set(visutiles_PROG_NAME_EXEC "visutiles")
configure_file(
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_SOURCE_DIR}/src/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O3")

# set(dkasses_compile_options_else
#     -Wall -Wextra -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast
#     -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion
#     -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond
#     -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast
#     -Wdouble-promotion -Wformat=2)



file(GLOB SOURCES "src/*.cpp" "src/third_party/tinyxml/tinyxml2.cpp")

set(EXECUTABLE_NAME "visutiles")
add_executable(visutiles ${SOURCES})
#target_compile_options(visutiles PRIVATE ${dkasses_compile_options_else})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME}  ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${visutiles_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${visutiles_VERSION_MINOR}")
include(CPack)

# Add assets
file(COPY src/assets DESTINATION ${CMAKE_BINARY_DIR})