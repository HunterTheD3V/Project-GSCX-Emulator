add_library(gscx_gpu_rsx SHARED src/gpu_rsx.cpp)

target_include_directories(gscx_gpu_rsx PUBLIC ../../core/include)

target_link_libraries(gscx_gpu_rsx PRIVATE gscx_core)

set_target_properties(gscx_gpu_rsx PROPERTIES OUTPUT_NAME "gscx_gpu_rsx")