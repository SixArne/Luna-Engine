set(CMAKE_CXX_STANDARD 20)

add_executable(Game
    Main.cpp

    # Components
    Components/FPSCounter.cpp
    Components/HealthComponent.cpp
    Components/HealthIndicator.cpp
    Components/PointComponent.cpp
    Components/PointIndicator.cpp

    # Input
    Input/DieCommand.cpp
    Input/MoveCommand.cpp
    Input/PointCommand.cpp
)

set_target_properties(Game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")

target_link_libraries(Game
    Engine
)
 
set(GameIncludes
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EngineCodeIncludeDir}
)

foreach(gameInclude in ${GameIncludes})
    message(STATUS "Including: ${gameInclude} in game")
endforeach()

target_include_directories(Game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EngineCodeIncludeDir}
)


add_dependencies(Game Engine)

# copy dlls to build directory
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)

# copy data folder to build directory
add_custom_command(TARGET Game POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Game/Data" "$<TARGET_FILE_DIR:Game>/Data"
)
