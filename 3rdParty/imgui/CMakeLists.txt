
# Gather all files
message(STATUS "include directory ${CMAKE_CURRENT_SOURCE_DIR}")

# Define library
add_library(imgui
    "imgui.h"
    "imconfig.h"
    "imgui_internal.h"
    "imstb_rectpack.h"
    "imstb_textedit.h"
    "imstb_truetype.h"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_tables.cpp"
    "imgui_widgets.cpp"
    "imgui.cpp"
    "backends/imgui_impl_opengl3.h"
    "backends/imgui_impl_opengl3.cpp"
    "backends/imgui_impl_sdl2.h"
    "backends/imgui_impl_sdl2.cpp"
)

set(imguiIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)