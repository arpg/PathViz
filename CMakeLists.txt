cmake_minimum_required(VERSION 2.8)
project(PathViz)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

if (POLICY CMP0062)
  cmake_policy(SET CMP0062 OLD)
endif()

set(PATHVIZ_VERSION_MAJOR 0)
set(PATHVIZ_VERSION_MINOR 1)
set(PATHVIZ_VERSION ${PATHVIZ_VERSION_MAJOR}.${PATHVIZ_VERSION_MINOR})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

find_package(PkgConfig REQUIRED)

find_package(GLEW REQUIRED)
list(APPEND REQUIRED_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS})
list(APPEND REQUIRED_LIBRARIES ${GLEW_LIBRARIES})

pkg_search_module(GLFW REQUIRED glfw3)
list(APPEND REQUIRED_INCLUDE_DIRS ${GLFW_INCLUDE_DIRS})
list(APPEND REQUIRED_LIBRARIES ${GLFW_LIBRARIES})

find_package(OpenGL REQUIRED)
list(APPEND REQUIRED_INCLUDE_DIRS ${OPENGL_INCLUDE_DIRS})
list(APPEND REQUIRED_LIBRARIES ${OPENGL_LIBRARIES})

find_package(OpenCV REQUIRED)
list(APPEND REQUIRED_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
list(APPEND REQUIRED_LIBRARIES ${OpenCV_LIBRARIES})

find_package(Calibu REQUIRED)
list(APPEND REQUIRED_INCLUDE_DIRS ${Calibu_INCLUDE_DIRS})
list(APPEND REQUIRED_LIBRARIES ${Calibu_LIBRARIES})

find_package(Sophus REQUIRED)
list(APPEND REQUIRED_INCLUDE_DIRS ${Sophus_INCLUDE_DIRS})
list(APPEND REQUIRED_LIBRARIES ${Sophus_LIBRARIES})

list(APPEND REQUIRED_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)
include_directories(${REQUIRED_INCLUDE_DIRS})

set(INC
  include/pathviz/Camera.h
  include/pathviz/Context.h
  include/pathviz/Exception.h
  include/pathviz/FrameBuffer.h
  include/pathviz/Image.h
  include/pathviz/ImageWriter.h
  include/pathviz/OpenGL.h
  include/pathviz/Path.h
  include/pathviz/PathReader.h
  include/pathviz/Program.h
  include/pathviz/Rig.h
  include/pathviz/RigBuilder.h
  include/pathviz/Scene.h
  include/pathviz/SceneBuilder.h
  include/pathviz/SceneRenderer.h
  include/pathviz/SceneVertexBuilder.h
  include/pathviz/Shader.h
  include/pathviz/Texture.h
  include/pathviz/TextureGenerator.h
  include/pathviz/Types.h
  include/pathviz/Util.h
  include/pathviz/VertexArray.h
)

set(SRC
  src/Camera.cpp
  src/Context.cpp
  src/FrameBuffer.cpp
  src/ImageWriter.cpp
  src/Path.cpp
  src/PathReader.cpp
  src/Program.cpp
  src/Rig.cpp
  src/RigBuilder.cpp
  src/Scene.cpp
  src/SceneBuilder.cpp
  src/SceneRenderer.cpp
  src/SceneVertexBuilder.cpp
  src/Shader.cpp
  src/Texture.cpp
  src/TextureGenerator.cpp
  src/VertexArray.cpp
)

foreach(INC_FILE ${INC})
  string(REPLACE "include/" "" INC_FILE ${INC_FILE})
  set(PATHVIZ_INCLUDES "${PATHVIZ_INCLUDES}\n#include <${INC_FILE}>")
endforeach(INC_FILE)

configure_file(
  include/pathviz/PathViz.h.in
  include/pathviz/PathViz.h
)

add_library(pathviz ${SRC} ${INC})
target_link_libraries(pathviz ${REQUIRED_LIBRARIES})

include(install_package)

install_package(
  PKG_NAME PATHVIZ
  LIB_NAME pathviz
  VERSION  ${PATHVIZ_VERSION}
  DESCRIPTION "Library for rendering synthetic scenes for a given path"
  INSTALL_INCLUDE_DIR true
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  INCLUDE_DIRS ${REQUIRED_INCLUDE_DIRS}
  LINK_LIBS ${REQUIRED_LIBRARIES}
)

if(NOT EXPORT_${PROJECT_NAME})
  set(PATHVIZ_SHADER_DIR "${CMAKE_INSTALL_PREFIX}/shader/")
elseif(EXPORT_${PROJECT_NAME})
  set(PATHVIZ_SHADER_DIR "${CMAKE_BINARY_DIR}/shader/")
endif()

set(DPATHVIZ_SHADER_DIR "\"${PATHVIZ_SHADER_DIR}\"")

add_definitions(-DPATHVIZ_SHADER_DIR=${DPATHVIZ_SHADER_DIR})

set(SHADERS
  texture.frag
  texture.vert
  triangle.frag
  triangle.vert
)

foreach(SHADER ${SHADERS})
  configure_file(shader/${SHADER} ${PATHVIZ_SHADER_DIR}${SHADER} COPYONLY)
endforeach(SHADER)

option(BUILD_APPLICATIONS "Build Examples" OFF)

if(${BUILD_APPLICATIONS})
  add_subdirectory(applications)
endif()
