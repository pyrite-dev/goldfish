# GoldFish Build
Usage: - [premake5](https://premake.github.io/download/)
       - [gmake](https://www.gnu.org/software/make/manual/make.html#Rules) <- Optional(However, it is required when using GCC or MinGW.)
       - [gcc](https://gcc.gnu.org/) or [MSVC](https://visualstudio.microsoft.com/ja/vs/features/cplusplus/) or [MingW](https://www.mingw-w64.org/).

1. clone this repository and submodule.
  `git clone https://github.com/pyrite-dev/goldfish --recursive`
2. Creating Makefiles for each environment(For other environments, please refer to the help.)
  - gcc / mingw
    `premake5 --file=build.lua gmake`
  - msvc
    `premake5 --file=build.lua vs2022 --cc=msc --opengl=gdi`
3. build
  - gcc / mingw
    `make -j4`
  - msvc
    1. Open the generated `EntireGoldFish.sln`
    2. ![]()
    3. ![]()
