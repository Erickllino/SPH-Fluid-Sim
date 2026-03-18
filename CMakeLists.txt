cmake_minimum_required(VERSION 3.18)
project(SPH C CXX)

set(CMAKE_CXX_STANDARD 17)

# ── CUDA opcional ─────────────────────────────────────────────────────────────
include(CheckLanguage)
check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    set(CMAKE_CUDA_STANDARD 17)
    set(CMAKE_CUDA_ARCHITECTURES native)
    add_compile_definitions(CUDA_AVAILABLE)
    message(STATUS "CUDA encontrado: ${CMAKE_CUDA_COMPILER}")
else()
    message(STATUS "CUDA não encontrado — compilando só com OpenGL/CPU")
endif()

find_package(OpenGL REQUIRED)

# GLFW via submodule
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11     ON  CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/glfw)


# GLAD via arquivos commitados
add_library(glad STATIC extern/glad/src/glad.c)
target_include_directories(glad PUBLIC extern/glad/include)

add_executable(main src/main.cpp)

target_link_libraries(main PRIVATE
    OpenGL::GL
    glfw
    glad
)
