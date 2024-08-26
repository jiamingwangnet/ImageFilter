# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# CACHE variables
set(GLM_INCLUDE_PATH "" CACHE STRING "Path to the GLM include directory.")
set(GLFW_BIN_PATH "" CACHE STRING "Path to the GLFW binaries directory.")
set(GLFW_INCLUDE_PATH "" CACHE STRING "Path to the GLFW includes directory.")
set(OPENCV_INCLUDE_PATH "" CACHE STRING "Path to the OpenCV includes directory.")
set(OPENCV_LIB_PATH "" CACHE STRING "Path to the OpenCV libraries directory.")

message("Building ${CMAKE_BUILD_TYPE}")

message("----------PATHS----------")
message("GLM INCLUDE               : ${GLM_INCLUDE_PATH}")
message("GLFW BINARIES             : ${GLFW_BIN_PATH}")
message("GLFW INCLUDES             : ${GLFW_INCLUDE_PATH}")
message("OpenCV INCLUDES           : ${OPENCV_INCLUDE_PATH}")
message("OpenCV LIBRARIES          : ${OPENCV_LIB_PATH}")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ImageFilter")

# Include sub-projects.
add_subdirectory ("ImageFilter")
