add_library(glm INTERFACE)

message(STATUS "include directory ${CMAKE_CURRENT_SOURCE_DIR}")
set(glmIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)