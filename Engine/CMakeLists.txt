set (CMAKE_CXX_STANDARD 20)

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