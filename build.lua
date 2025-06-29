workspace("EntireGoldFish")
        flags("MultiProcessorCompile")
        configurations({
                "Debug",
                "Release"
        })
        platforms({
                "Native",
                "Win32",
                "Win64"
        })
        defaultplatform("Native")

        objdir("obj/%{cfg.platform}/%{cfg.buildcfg}")
        targetdir("lib/%{cfg.buildcfg}/%{cfg.platform}")
        debugdir("bin/%{cfg.buildcfg}/%{cfg.platform}")

filter("platforms:Win32")
        system("windows")
        architecture("x86")
        gccprefix("i686-w64-mingw32-")

filter("platforms:Win64")
        system("windows")
        architecture("x86_64")
        gccprefix("x86_64-w64-mingw32-")

filter({})

include(".")
