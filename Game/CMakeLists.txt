set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler warnings as errors
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "[Game] Setting warnings as errors for GNU/Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
elseif(MSVC)
    message(STATUS "[Game] Setting warnings as errors for MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

add_executable(Game
    Main.cpp

    # Components
    Components/FPSCounter.cpp
    Components/HealthComponent.cpp
    Components/HealthIndicator.cpp
    Components/PointComponent.cpp
    Components/PointIndicator.cpp

    Components/LivesIndicator.cpp
    Components/HighScoreIndicator.cpp
    Components/Player/SpaceFighter.cpp
    Components/Player/Projectile.cpp
    Components/Util/AutoKill.cpp

    # Input
    Input/DieCommand.cpp
    Input/MoveCommand.cpp
    Input/PointCommand.cpp
    Input/ShootCommand.cpp
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

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode enabled. Optimizations disabled.")
    target_compile_definitions(Engine PUBLIC DEBUG=1)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release mode enabled")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

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
