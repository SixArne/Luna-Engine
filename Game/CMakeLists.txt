set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

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

FetchContent_Declare(
    rapidjson
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    GIT_TAG "master"
    DOWNLOAD_NO_PROGRESS ON
)

FetchContent_GetProperties(rapidjson)
if(NOT rapidjson_POPULATED)
    FetchContent_Populate(rapidjson)
    message(STATUS "Fetching spdlog from https://github.com/Tencent/rapidjson.git on branch master")
    message(STATUS "Setting RAPIDJSON_INCLUDE_DIR to ${rapidjson_SOURCE_DIR}")
    set(RAPIDJSON_INCLUDE_DIR "${rapidjson_SOURCE_DIR}")
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
    Components/Enemy/BaseEnemy.cpp
    Components/Enemy/EnemyBug.cpp
    Components/Util/AutoKill.cpp
    Components/Game/EnemyManager.cpp

    Components/Menu/MenuOption.cpp
    Components/Menu/OptionContainer.cpp

    AnimationStates/FlyInState.cpp
    AnimationStates/FlyBackState.cpp
    AnimationStates/AttackState.cpp
    AnimationStates/DeathState.cpp
    AnimationStates/IdleState.cpp

    LevelLoader/LevelLoader.cpp
    LevelLoader/ScoreReader.cpp
    LevelLoader/ScoreWriter.cpp
    LevelInstancer/LevelInstancer.cpp

    Util/BezierSpline.cpp
    Util/Random.cpp

    # Input
    Input/DieCommand.cpp
    Input/MoveCommand.cpp
    Input/PointCommand.cpp
    Input/ShootCommand.cpp
    Input/Utils/SwitchSceneCommand.cpp
    Input/Utils/NavigateMenuCommand.cpp
    Input/Utils/SoundVolumeCommand.cpp
)

if(MSVC)
 target_compile_options(Game PRIVATE "/MP")
endif()

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
    ${RAPIDJSON_INCLUDE_DIR}/include
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
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_SMPEG2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)

# MIXER DLLS
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBFLAC_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBMIKMOD_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBMODPLUG_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBOGG_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBVORBIS_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)
add_custom_command(TARGET Game POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBVORBISFILE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:Game>"
)

# copy data folder to build directory
add_custom_command(TARGET Game POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Game/Data" "$<TARGET_FILE_DIR:Game>/Data"
)
