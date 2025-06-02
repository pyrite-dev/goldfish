flags("MultiProcessorCompile")

workspace("Examples")
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

filter("platforms:Win32")
	system("windows")
	architecture("x86")
	gccprefix("i686-w64-mingw32-")

filter("platforms:Win64")
	system("windows")
	architecture("x86_64")
	gccprefix("x86_64-w64-mingw32-")

filter({})

include ".."

function call_me(name)
	project(name)
		flags("RelativeLinks")
		kind("ConsoleApp")
		language("C")
		targetdir("bin/%{cfg.buildcfg}/%{cfg.platform}")
		objdir("obj")
		targetname(name)
		includedirs({
			"../include"
		})
		files({
			"src/" .. name .. ".c"
		})
		links({
			"GoldFish"
		})
		-- Call this if you are gonna use my engine...
		gf_link_stuffs("options:engine=static")
		filter("configurations:Debug")
			defines({
				"DEBUG"
			})
			symbols("On")
		filter("configurations:Release")
			defines({
				"NDEBUG"
			})
			optimize("On")
		filter({})
end

call_me("audio")
call_me("network")
