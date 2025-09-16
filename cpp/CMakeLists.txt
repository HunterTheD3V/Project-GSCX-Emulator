cmake_minimum_required(VERSION 3.20)
project(GSCX LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(core)
add_subdirectory(modules/cpu_cell)
add_subdirectory(modules/gpu_rsx)
add_subdirectory(modules/recovery)

# Ferramentas
add_executable(gscore_packer tools/gscore_packer.cpp)