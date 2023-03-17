# CMake

## How to clone

Please use the following command to make sure to get all dependencies.

>git clone https://github.com/SixArne/Luna-Engine -b cmake --recursive

## How to build

Run the CreateProject.bat file from the **root** project.

after that run the following command from **inside** the build folder.
>cmake --build . --config Debug --parallel -v -j 8


## Disclaimer
I have debugged most issues but I got stuck at
```
>MSVCRTD.lib(exe_main.obj) : error LNK2019: unresolved external symbol main referenced in function "int __cdecl invoke_main(void)" (?invoke_main@@YAHXZ) [C:\dev\Luna-Engine\Build\Engine\LunaEngine.vcxproj]
     3>C:\dev\Luna-Engine\Build\Engine\Debug\LunaEngine.exe : fatal error LNK1120: 1 unresolved externals [C:\dev\Luna-Engine\Build\Engine\LunaEngine.vcxproj]
```

Which seems super cryptic.

I feel like this is the last thing that needs to be linked in to fully convert this to use cmake.

I typically run this command to build everything.
```
cmake --build . --config Debug --parallel -v -j 8
```

I have narrowed the problem down to linking SDL into LunaEngine.