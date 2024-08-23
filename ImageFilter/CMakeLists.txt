# CMakeList.txt : CMake project for ImageFilter, include source and define
# project specific logic here.
#

file(GLOB exec_src CONFIGURE_DEPENDS
        "*.cpp"
        "stb/stb_image.cpp"
        "glad/src/gl.c"
        "imgui/*.cpp"
)

file(GLOB exec_head CONFIGURE_DEPENDS
        "include/*.h"
        "glad/include/glad/gl.h"
        "stb/include/stb/stb_image.h"
        "imgui/*.h"
        "${GLM_INCLUDE_PATH}"
        "${GLFW_INCLUDE_PATH}"
)

add_executable(ImageFilter ${exec_src} ${exec_head})

target_include_directories(ImageFilter PRIVATE "${GLM_INCLUDE_PATH}")
target_include_directories(ImageFilter PRIVATE "${GLFW_INCLUDE_PATH}")
target_include_directories(ImageFilter PRIVATE "glad/include")
target_include_directories(ImageFilter PRIVATE "stb/include")

target_link_libraries(ImageFilter "${GLFW_BIN_PATH}/glfw3dll.lib")

set_property(TARGET ImageFilter PROPERTY CXX_STANDARD 20)

# add_custom_command(TARGET ImageFilter POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     "${GLFW_BIN_PATH}/glfw3.dll"
#     "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>")

# TODO: Add tests and install targets if needed.
