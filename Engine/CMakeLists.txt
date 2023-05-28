set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler warnings as errors
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "[Engine] Setting warnings as errors for GNU/Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
elseif(MSVC)
    message(STATUS "[Engine] Setting warnings as errors for MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

# Include fetchcontent
include(FetchContent)

set(EngineSourceFiles
  Texture2D.cpp
  SceneManager.cpp
  Scene.cpp
  ResourceManager.cpp
  Renderer.cpp
  pch.cpp
  GameObject.cpp
  Font.cpp
  Engine.cpp
  Core/Log.cpp
  Core/Time.cpp
  Core/ECS/Component.cpp
  Core/ECS/TextComponent.cpp
  Core/ECS/TextureRenderer.cpp
  Core/ECS/TransformComponent.cpp
  Core/ECS/SpriteAnimator.cpp
  Core/ECS/RigidBody2D.cpp
  Core/Event/EventManager.cpp
  "Core/Input/Command.cpp"
  "Core/Input/InputManager.cpp"
  "Core/Input/Keyboard.cpp"
  "Core/Input/XboxController.cpp"

  Core/Services/ServiceLocator.cpp
  Core/Services/Sound/LoggingSoundSystem.cpp
  Core/Services/Sound/SDLSoundSystem.cpp
  Core/Services/Physics/PhysicsService.cpp
)

foreach(engineSrcFile in ${EngineSourceFiles})
  message(STATUS "Adding source file ${engineSrcFile} to Engine")
endforeach()


# files to compile
add_library(Engine STATIC
  ${EngineSourceFiles}
)

# set target settings
target_compile_features(Engine PUBLIC cxx_std_20)
target_compile_definitions(Engine PUBLIC SDL_MAIN_HANDLED)
target_compile_options(Engine PRIVATE /W4 /WX)

# Fetch libraries

# SDL2
FetchContent_Declare(
  SDL2
  URL https://www.libsdl.org/release/SDL2-devel-2.26.3-VC.zip
  DOWNLOAD_NO_PROGRESS ON
)

FetchContent_GetProperties(SDL2)
if(NOT SDL2_POPULATED)
  FetchContent_Populate(SDL2)
  set(SDL2_POPULATED true)
  set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
  set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
else()
  set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib")
  set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
endif()
endif()

# add SDL2_image
FetchContent_Declare(
  SDL2_image
  URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.6.3-VC.zip
  DOWNLOAD_NO_PROGRESS ON
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_image)
if(NOT SDL2_image_POPULATED)
  FetchContent_Populate(SDL2_image)
  set(SDL2_IMAGE_INCLUDE_DIR ${sdl2_image_SOURCE_DIR}/include)
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.lib")
  set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.dll")
else()
  set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.lib")
  set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.dll")
endif()
endif()

# add SDL2_mixer
FetchContent_Declare(
  SDL2_mixer
  URL https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.0-VC.zip
  DOWNLOAD_NO_PROGRESS ON
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_mixer)
if(NOT SDL2_mixer_POPULATED)
  FetchContent_Populate(SDL2_mixer)
  set(SDL2_MIXER_INCLUDE_DIR ${sdl2_mixer_SOURCE_DIR}/include)
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_MIXER_LIBRARIES "${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.lib")
  set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.dll")
  set(SDL2_MIXER_SMPEG2_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/smpeg2.dll")

  set(SDL2_MIXER_LIBFLAC_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/libFLAC-8.dll")
  set(SDL2_MIXER_LIBMIKMOD_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/libmikmod-2.dll")
  set(SDL2_MIXER_LIBMODPLUG_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/libmodplug-1.dll")
  set(SDL2_MIXER_LIBOGG_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/libogg-0.dll")
  set(SDL2_MIXER_LIBVORBIS_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/libvorbis-0.dll")
  set(SDL2_MIXER_LIBVORBISFILE_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/libvorbisfile-3.dll")

else()
  set(SDL2_MIXER_LIBRARIES "${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.lib")
  set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.dll")
  set(SDL2_MIXER_SMPEG2_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/smpeg2.dll")

  set(SDL2_MIXER_LIBFLAC_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/libFLAC-8.dll")
  set(SDL2_MIXER_LIBMIKMOD_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/libmikmod-2.dll")
  set(SDL2_MIXER_LIBMODPLUG_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/libmodplug-1.dll")
  set(SDL2_MIXER_LIBOGG_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/libogg-0.dll")
  set(SDL2_MIXER_LIBVORBIS_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/libvorbis-0.dll")
  set(SDL2_MIXER_LIBVORBISFILE_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/libvorbisfile-3.dll")

endif()
endif()

# add SDL2_ttf
FetchContent_Declare(
  SDL2_ttf
  URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.20.2-VC.zip
  DOWNLOAD_NO_PROGRESS ON
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)
FetchContent_GetProperties(SDL2_ttf)
if(NOT sdl2_ttf_POPULATED)
  FetchContent_Populate(SDL2_ttf)
  set(SDL2_TTF_INCLUDE_DIR ${sdl2_ttf_SOURCE_DIR}/include)
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.lib)
  set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.dll")
else()
  set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.lib)
  set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.dll")
endif()
endif()

set(VLD_DIR "$ENV{ProgramFiles\(x86\)}/Visual Leak Detector")
IF(EXISTS ${VLD_DIR})
  set(VLD_INCLUDE_DIR "${VLD_DIR}/include")
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
      set(VLD_LIBRARY "${VLD_DIR}/lib/Win64/vld.lib")
  else()
      set(VLD_LIBRARY "${VLD_DIR}/lib/Win32/vld.lib")
  endif()
endif()

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/SixArne/glm.git
    GIT_TAG "master"
    DOWNLOAD_NO_PROGRESS ON
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    message(STATUS "Fetching glm from https://github.com/SixArne/glm.git on branch master")
    message(STATUS "Setting GLM_INCLUDE_DIR to ${glm_SOURCE_DIR}")
    set(GLM_INCLUDE_DIR "${glm_SOURCE_DIR}")
endif()

# Imgui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/SixArne/imgui.git
    GIT_TAG "docking"
    DOWNLOAD_NO_PROGRESS ON
)

FetchContent_MakeAvailable(imgui)
# Print imgui target properties

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set (IMGUI_LIB ${CMAKE_BINARY_DIR}/_deps/imgui-build/Release/IMGUI.lib)
else()
  set (IMGUI_LIB ${CMAKE_BINARY_DIR}/_deps/imgui-build/Debug/IMGUI.lib)
endif()
message(STATUS "IMGUI lib path: ${IMGUI_LIB}")

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/SixArne/spdlog.git
    GIT_TAG "v1.x"
    DOWNLOAD_NO_PROGRESS ON
)

FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    message(STATUS "Fetching spdlog from https://github.com/SixArne/spdlog.git on branch v1.x")
    message(STATUS "Setting SPDLOG_INCLUDE_DIR to ${spdlog_SOURCE_DIR}")
    set(SPDLOG_INCLUDE_DIR "${spdlog_SOURCE_DIR}")
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

set_target_properties(IMGUI PROPERTIES
  CMAKE_BUILD_TYPE Release
)

set (EngineIncludes
    ${VLD_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${RAPIDJSON_INCLUDE_DIR}/include
    ${SPDLOG_INCLUDE_DIR}/include
)

foreach(engineInclude in ${EngineIncludes})
    message(STATUS "Including: ${engineInclude} in Engine")
endforeach()

target_include_directories(Engine PUBLIC
    # self include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EngineIncludes}
)


set (EngineLinks
  ${SDL2_LIBRARIES}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${SDL2_MIXER_LIBRARIES}
  ${VLD_LIBRARY}
  ${IMGUI_LIB}
)

foreach(engineLink in ${EngineLinks})
    message(STATUS "Linking: ${engineLink} in Engine")
endforeach()


target_link_libraries(Engine
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${VLD_LIBRARY}
    ${IMGUI_LIB}
    xinput.lib
    opengl32.lib
    # THIS IMGUI NEEDS TO BUILD IN RELEASE MODE
)

add_dependencies(Engine IMGUI)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(Engine PUBLIC DEBUG=1)
endif()

# Set SDL2 dlls as properties
set (SDL2_LIBRARY_DLL ${SDL2_LIBRARY_DLL} PARENT_SCOPE)
set (SDL2_IMAGE_LIBRARY_DLL ${SDL2_IMAGE_LIBRARY_DLL} PARENT_SCOPE)
set (SDL2_TTF_LIBRARY_DLL ${SDL2_TTF_LIBRARY_DLL} PARENT_SCOPE)
set (SDL2_MIXER_LIBRARY_DLL ${SDL2_MIXER_LIBRARY_DLL} PARENT_SCOPE)
set (SDL2_MIXER_SMPEG2_LIBRARY_DLL ${SDL2_MIXER_SMPEG2_LIBRARY_DLL} PARENT_SCOPE)

set(SDL2_MIXER_LIBFLAC_LIBRARY_DLL ${SDL2_MIXER_LIBFLAC_LIBRARY_DLL} PARENT_SCOPE)
set(SDL2_MIXER_LIBMIKMOD_LIBRARY_DLL ${SDL2_MIXER_LIBMIKMOD_LIBRARY_DLL} PARENT_SCOPE)
set(SDL2_MIXER_LIBMODPLUG_LIBRARY_DLL ${SDL2_MIXER_LIBMODPLUG_LIBRARY_DLL} PARENT_SCOPE)
set(SDL2_MIXER_LIBOGG_LIBRARY_DLL ${SDL2_MIXER_LIBOGG_LIBRARY_DLL} PARENT_SCOPE)
set(SDL2_MIXER_LIBVORBIS_LIBRARY_DLL ${SDL2_MIXER_LIBVORBIS_LIBRARY_DLL} PARENT_SCOPE)
set(SDL2_MIXER_LIBVORBISFILE_LIBRARY_DLL ${SDL2_MIXER_LIBVORBISFILE_LIBRARY_DLL} PARENT_SCOPE)

set (EngineCodeIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)