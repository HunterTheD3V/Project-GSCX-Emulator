add_library(gscx_cpu_cell SHARED src/cpu_cell.cpp)

target_include_directories(gscx_cpu_cell PUBLIC ../../core/include)

target_link_libraries(gscx_cpu_cell PRIVATE gscx_core)

set_target_properties(gscx_cpu_cell PROPERTIES OUTPUT_NAME "gscx_cpu_cell")