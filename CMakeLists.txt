cmake_minimum_required(VERSION 3.10)

project(LunaEngineProject)
set(Author "Six Arne")
set(Project "Luna Engine")

add_subdirectory(3rdParty/imgui)
add_subdirectory(3rdParty/implot)
add_subdirectory(3rdParty/glm)
add_subdirectory(3rdParty/spdlog)
add_subdirectory(Engine)

message(STATUS "include directory ${PROJECT_SOURCE_DIR}")