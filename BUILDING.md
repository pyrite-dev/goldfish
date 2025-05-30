# Building GoldFish
Requirements:
 - [premake5](https://premake.github.io/download/)
 - [gmake](https://www.gnu.org/software/make/manual/make.html) (Not required for MSVC build.)
 - [GCC](https://gcc.gnu.org/), [MSVC](https://visualstudio.microsoft.com/ja/vs/features/cplusplus/) or [MinGW](https://www.mingw-w64.org/).

## Steps

1. Clone the repository

   `git clone https://github.com/pyrite-dev/goldfish --recursive`
2. Create platform-dependent build files

   See `--help` for more information about build configuration

   - GCC / MinGW

     `premake5 --file=build.lua gmake`
   - MSVC

     `premake5 --file=build.lua vs(year) --cc=msc --opengl=gdi`
     
     (year) should be replaced with your Visual Studio version of choice, for example if you want to build the project with VS2008, write vs2008 here.
  
     In case you don't have `--cc=msc` as a valid option if you're e.g using the final release of MSYS2 for Windows 7, simply omit `-cc=msc` from the parameters, It won't affect anything.

     Visual Studio 2008 is the minimum supported version and Visual Studio 2005 is the oldest version that can build GoldFish.
3. Build
  - GCC

    `make -j4`
  - MinGW

    `make -j4 config=<config>_<platform>`
    
    `<config>` must be `debug` or `release`, and `<platform>` must be `win32` or `win64`.

  - MSVC

    There are multiple ways to build GoldFish on MSVC.

    #### Method 1: Graphical
    Open the EntireGoldFish.sln solution file and then build the entire solution by pressing Ctrl-Shift-B on your keyboard, or alternatively use the Build drop-down menu and then select `Build Solution`.

    #### Method 2: MSBuild
    Build the entire solution by running `MSBuild EntireGoldFish.sln /t:Build /p:Configuration=<gooboo>`.

    `<gooboo>` must be either `Debug` or `Release`.
