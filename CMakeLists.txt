project(mygame)
cmake_minimum_required(VERSION 2.8)

file(GLOB SRCS src/*.cpp)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

add_definitions("-g -Wall -Wextra -Weffc++ -pedantic")

add_executable(mygame ${SRCS})

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS system graphics window audio network)
find_package(OpenGL)
find_package(GLEW)
find_package(JPEG)
find_package(OpenAL)


IF(APPLE)
  INCLUDE_DIRECTORIES ( /Developer/Headers/FlatCarbon )
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(CARBON_LIBRARY Carbon)
  FIND_LIBRARY(IOKIT_LIBRARY IOKit )
  MARK_AS_ADVANCED (CARBON_LIBRARY
    IOKIT_LIBRARY
    COCOA_LIBRARY)
  SET(EXTRA_LIBS ${CARBON_LIBRARY} ${IOKIT_LIBRARY} ${COCOA_LIBRARY})
ENDIF (APPLE)


include_directories(
  ${SFML_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
)

target_link_libraries(mygame
  ${SFML_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${OPENAL_LIBRARY}
  ${GLEW_LIBRARY}
  ${JPEG_LIBRARIES}
  ${EXTRA_LIBS}
)

file(COPY resources DESTINATION .)
