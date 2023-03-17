
# Gather all files
message(STATUS "include directory ${CMAKE_CURRENT_SOURCE_DIR}")

# Define library
add_library(implot STATIC
    "implot.cpp"
    "implot.h"
    "implot_demo.cpp"
    "implot_internal.h"
    "implot_items.cpp"
)

target_include_directories(imgui PUBLIC
    # self root reference
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(implot PUBLIC imgui)

set(implotIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)