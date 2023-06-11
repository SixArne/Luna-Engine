# Luna Engine


## Goal

The goal of this engine is to be used for simple retro arcade games, as it only covers the bare basics for a 2D engine.
My future plan is to write a full 3D/2D engine that supports more features in the future.


## Disclaimer

This engine is for me to get familier with the design process. This engine may contains some questionable choices but has provided me with a bigger insight for my next engine project.

# Project link:

https://github.com/SixArne/Luna-Engine

# Features

- Component system
- Collision detection
- Audio
- Input system with schema's
- CMake so any platform can build this

## Threading

Engine has a total of 3 threads

- Collision thread
    Because of the quadratic nature of collision detection I've decided to put this in a seperate thread.

- Audio thread
    ~ Audio related stuff

- Main thread
    ~ Everything else


# Installation

Preferably use Visual Studio Code for this project.

## Setup CMake for Debug/Release

By default the project will be setup for debug mode. If you want to setup
the project for release use the following tasks.

`ctrl + P`

`Run build task`

`Configure`

`Debug | Release`

This will tell cmake to prepare for a different configuration

## Compiling the project

You can compule the project by using:

`ctrl + P`

`Run build task`

`Debug | Release`

This will compile the projects and put them in the build folder.

## Not using visual studio code?

Go to the scripts folder and first use

`GenerateProjectDebug.bat` or `GenerateProjectRelease.bat`.

then use

`BuildProjectDebug.bat` or `BuildProjectRelease.bat`.



