project(mygame)
set(NAME mygame)
cmake_minimum_required(VERSION 2.8)

file(GLOB SRCS src/*.cpp src/*.h)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions("-g -Wall -Wextra -Weffc++ -pedantic")
endif()

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if(APPLE)
  file(GLOB OBJC_FILES src/*.mm)
  list(APPEND SRCS ${OBJC_FILES})

  file(GLOB RESOURCE_FILES resources/*)
  set_source_files_properties( ${RESOURCE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  list(APPEND SRCS ${RESOURCE_FILES})

endif()

# Dependencies

set(SFMLDIR external)
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS system graphics window audio network)
find_package(OpenGL)

if(UNIX)
  find_package(GLEW)
else()
  list(APPEND SRCS src/glew.c)
endif()

if(APPLE)
  find_package(GLEW)
  find_package(JPEG)
endif()
#find_package(OpenAL)

if(APPLE)
  include_directories(/Developer/Headers/FlatCarbon)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(CARBON_LIBRARY Carbon)
  find_library(IOKIT_LIBRARY IOKit)
  mark_as_advanced (CARBON_LIBRARY IOKIT_LIBRARY COCOA_LIBRARY)
  set(EXTRA_LIBS ${CARBON_LIBRARY} ${IOKIT_LIBRARY} ${COCOA_LIBRARY})
endif(APPLE)

include_directories(
  ${SFML_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
  ${JPEG_INCLUDE_DIR}
)

add_executable(mygame WIN32 MACOSX_BUNDLE ${SRCS})

target_link_libraries(mygame
  ${SFML_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${OPENAL_LIBRARY}
  ${GLEW_LIBRARY}
  ${JPEG_LIBRARIES}
  ${EXTRA_LIBS}
)

file(COPY resources DESTINATION .)

# Installation and packaging

install(TARGETS mygame DESTINATION .)
install(DIRECTORY resources DESTINATION .)

if (APPLE)
  SET(CPACK_BINARY_OSXX11 "ON")
  set(APP_TYPE MACOSX_BUNDLE)

  #set_target_properties(${NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif()

SET(CPACK_GENERATOR "ZIP")
include(CPack)
