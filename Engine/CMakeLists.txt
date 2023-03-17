add_executable(LunaEngine Main.cpp)

message(STATUS "include directory ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(LunaEngine PUBLIC
    "${glmIncludeDir}"
    "${imguiIncludeDir}"
    "${spdlogIncludeDir}"
    "${implotIncludeDir}"
)