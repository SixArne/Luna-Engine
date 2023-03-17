cmake_minimum_required(VERSION 3.10)

project(LunaEngineProject)

set(CMAKE_BUILD_TYPE Release)

set(Author "Six Arne")
set(Project "Luna Engine")

add_subdirectory(3rdParty/SDL2)
add_subdirectory(3rdParty/SDL_ttf)
add_subdirectory(3rdParty/SDL_image)
add_subdirectory(3rdParty/imgui)
add_subdirectory(3rdParty/implot)
add_subdirectory(3rdParty/glm)
add_subdirectory(3rdParty/spdlog)
add_subdirectory(Engine)

install(TARGETS SDL2 DESTINATION bin)
install(TARGETS SDL2_image DESTINATION bin)
install(TARGETS SDL2_ttf DESTINATION bin)
install(TARGETS imgui ARCHIVE DESTINATION bin)
install(TARGETS implot DESTINATION bin)

# tell that imgui needs SDL2 to be build first
add_dependencies(imgui SDL2)
add_dependencies(implot imgui)
add_dependencies(LunaEngine
    imgui
    implot
    SDL2
    SDL2_image
    SDL2_ttf
    spdlog
    glm
)

install(TARGETS imgui DESTINATION bin)

message(STATUS "include directory ${PROJECT_SOURCE_DIR}")