add_library(spdlog INTERFACE)

message(STATUS "include directory ${CMAKE_CURRENT_SOURCE_DIR}")
set(spdlogIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)

