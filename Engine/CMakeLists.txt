set (CMAKE_CXX_STANDARD 20)

#TODO - Add copying Data folder and copy DLL files

add_executable(LunaEngine
    "Texture2D.h"
    "Engine.cpp"
    "Engine.h"
    "Font.cpp"
    "Font.h"
    "GameObject.cpp"
    "GameObject.h"
    "InputManager.cpp"
    "InputManager.h"
    "Main.cpp"
    "pch.cpp"
    "pch.h"
    "Renderer.cpp"
    "Renderer.h"
    "ResourceManager.cpp"
    "ResourceManager.h"
    "SceneManager.cpp"
    "SceneManager.h"
    "Singleton.h"
    "Texture2D.cpp"
    "Util/OBJParser.cpp"
    "Util/OBJParser.h"
    "Scenes/RotatorScene.cpp"
    "Scenes/RotatorScene.h"
    "Scenes/Scene.cpp"
    "Scenes/Scene.h"
    "Core/Log.cpp"
    "Core/Log.h"
    "Core/Time.cpp"
    "Core/Time.h"
    "Core/ECS/Component.cpp"
    "Core/ECS/Component.h"
    "Core/ECS/TextComponent.cpp"
    "Core/ECS/TextComponent.h"
    "Core/ECS/TextureRenderer.cpp"
    "Core/ECS/TextureRenderer.h"
    "Core/ECS/TransformComponent.cpp"
    "Core/ECS/TransformComponent.h"
    "Core/ECS/Custom/FPSCounter.cpp"
    "Core/ECS/Custom/FPSCounter.h"
    "Core/ECS/Custom/Rotator.cpp"
    "Core/ECS/Custom/Rotator.h"
)

# Copy SDL2 DDL
add_custom_target(copy_SDL_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL2> $<TARGET_FILE_DIR:LunaEngine>
    COMMENT "Copying SDL2 DLL files to LunaEngine directory"
)

# Copy SDL2_image DLL
add_custom_target(copy_SDL_image_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL2_image> $<TARGET_FILE_DIR:LunaEngine>
    COMMENT "Copying SDL2_image DLL files to LunaEngine directory"
)

# Copy SDL2_ttf DLL
add_custom_target(copy_SDL_ttf_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SDL2_ttf> $<TARGET_FILE_DIR:LunaEngine>
    COMMENT "Copying SDL2_ttf DLL files to LunaEngine directory"
)

# Copy data folder
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(dest_dir "${CMAKE_CURRENT_BINARY_DIR}/Debug")
else()
    set(dest_dir "${CMAKE_CURRENT_BINARY_DIR}/Release")
endif()

# Copy the Data directory to the destination directory
file(COPY "${Root}/Data" DESTINATION "${dest_dir}")

# link all in
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(LunaEngine PUBLIC
        SDL2d
        SDL2_ttfd
        SDL2_imaged
        imgui
        implot
    )
else()
    target_link_libraries(LunaEngine PUBLIC
        SDL2
        SDL2_ttf
        SDL2_image
        imgui
        implot
    )
endif()

message(STATUS "include directory ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(LunaEngine PUBLIC
    # self root reference
    "${CMAKE_CURRENT_SOURCE_DIR}"

    # Dunno how to properly do these
    "../3rdParty/SDL2/include/"
    "../3rdParty/SDL_ttf/"
    "../3rdParty/SDL_image/"

    # I do know how to do these?
    "${glmIncludeDir}"
    "${imguiIncludeDir}"
    "${spdlogIncludeDir}"
    "${implotIncludeDir}"
)