require("gmake")
if premake.modules.gmake.cpp.pchRules and not(premake.modules.gmake.patched_resource) then
	premake.modules.gmake.patched_resource = true
	premake.override(premake.modules.gmake.cpp, "pchRules", function(base, cfg, toolset)
		base(cfg, toolset)
		_p("$(RESOURCES): | prebuild")
	end)
end

gf_sound_backends = {
	miniaudio = {
		name = "miniaudio"
	},
	alsa = {
		name = "ALSA",
		links = {"asound"}
	},
	oss = {
		name = "OSS",
		links = {"ossaudio"}
		-- I don't know how this works on non-NetBSD systems, I am sorry
	},
	sun = {
		name = "Sun/audio(4)"
	},
	sdl2 = {
		name = "SDL2",
		config = "sdl2"
	},
	null = {
		name = "Null"
	},
	windows = {
		name = "DirectSound/WinMM",
	},
	winmm = {
		name = "WinMM",
	}
}

gf_backends = {
	opengl = {
		name = "OpenGL",
		default_backend = "rgfw",
		default_type = "native",
		types = {
			native = {
				name = "Native",
				windows = {"opengl32"},
				unix = {"GL"}
			},
			osmesa = {
				name = "OSMesa",
				includedirs = {
					"external/osmesa/include",
					"external/osmesa/src",
					"external/osmesa/src/main",
					"external/osmesa/src/glapi",
					"external/osmesa/src/shader",
					"external/osmesa/src/shader/grammar",
					"external/osmesa/src/shader/slang"
				},
				files = {
					"external/osmesa/src/**.c",
					"-external/osmesa/src/shader/grammar/grammar.c"
				}
			}
		},
		backends = {
			x11 = {
				name = "X11",
				links = {
					"X11"
				}
			},
			gdi = {
				name = "GDI",
				links = {
					"gdi32"
				}
			},
			rgfw = {
				name = "RGFW",
				includedirs = {
					"external/rgfw"
				}
			},
			agl = {
				name = "agl",
				includedirs = {
					"external/OpenGLOnMacOS9/include",
					"misc/classic_mac_os_hacks"
				},
				links = {
					-- "external/OpenGLOnMacOS9/lib/libgl.a"
				}
			},
			["rgfw-wayland"] = {
				alias = "rgfw",
				name = "RGFW (Wayland)",
				includedirs = {
					"external/rgfw"
				},
				defines = {
					"RGFW_WAYLAND=1"
				},
				links = {
					"wayland-cursor",
					"wayland-client",
					"xkbcommon",
					"wayland-egl",
					"EGL",
					"GL"
				},
				files = {
					"external/rgfw/relative-pointer-unstable-v1-client-protocol.c",
					"external/rgfw/xdg-decoration-unstable-v1.c",
					"external/rgfw/xdg-shell.c"
				}
			}
		}
	}
}

gf_audio_backends = {
	miniaudio = {},
	none = {},
}

gf_l = {}
for k,v in pairs(gf_backends) do
	allowed = {}
	for k2,v2 in pairs(v["backends"]) do
		table.insert(allowed, {k2, v2["name"]})
	end
	newoption({
		trigger = k,
		value = "API",
		description = "Choose a backend for " .. v["name"],
		allowed = allowed,
		category = "Engine",
		default = v["default_backend"]
	})

	allowed = {}
	for k2,v2 in pairs(v["types"]) do
		table.insert(allowed, {k2, v2["name"]})
	end
	newoption({
		trigger = k .. "-type",
		value = "API",
		description = "Choose a type for " .. v["name"],
		allowed = allowed,
		category = "Engine",
		default = v["default_type"]
	})

	table.insert(gf_l, {k, v["name"]})
end


gf_aud = {}
for k,v in pairs(gf_audio_backends) do
	table.insert(gf_aud, {k, v})
end

newoption({
	trigger = "backend",
	value = "API",
	description = "Choose a backend for rendering",
	allowed = gf_l,
	category = "Engine",
	default = "opengl"
})

gf_l = {}
for k,v in pairs(gf_sound_backends) do
	table.insert(gf_l, {k, v["name"]})
end
newoption({
	trigger = "sound",
	value = "API",
	description = "Choose a sound backend",
	allowed = gf_l,
	category = "Engine",
	default = "miniaudio"
})

newoption({
	trigger = "engine",
	value = "type",
	description = "Choose an engine type",
	allowed = {
		{"static", "Static library"},
		{"dynamic", "Dynamic library"}
	},
	category = "Engine",
	default = "static"
})

newoption({
	trigger = "ode",
	value = "type",
	description = "Choose ODE to be used",
	allowed = {
		{"compile", "Compiled and integrated"},
		{"system", "System"}
	},
	category = "Engine",
	default = "compile"
})

newoption({
	trigger = "zlib",
	value = "type",
	description = "Choose zlib to be used",
	allowed = {
		{"compile", "Compiled and integrated"},
		{"system", "System"}
	},
	category = "Engine",
	default = "compile"
})

newoption({
	trigger = "pcre",
	value = "type",
	description = "Choose PCRE to be used",
	allowed = {
		{"compile", "Compiled and integrated"},
		{"system", "System"}
	},
	category = "Engine",
	default = "compile"
})

newoption({
	trigger = "server",
	value = "toggle",
	description = "Compile server",
	allowed = {
		{"yes", "Enable"},
		{"no", "Disable"}
	},
	category = "Engine",
	default = "yes"
})


gf_defs = {}
function gf_adddef(x)
	for _,v in ipairs(x) do
		table.insert(gf_defs, v)
	end
end
	
function gf_default_stuffs()
	local brk = false
	filter({})
if not(_OPTIONS["ode"] == "system") then
	defines({
		"HAVE_STDARG_H=1",
		"dIDEDOUBLE",
		"CCD_IDEDOUBLE"
	})
else
	defines({
		"HAVE_STDARG_H=1"
	})
end
	filter("system:not windows")
		defines({
			"HAVE_UNISTD_H=1"
		})
	filter("system:windows")
		defines({
			"WIN32"
		})
	for k,v in pairs(gf_sound_backends) do
		if _OPTIONS["sound"] == k then
			gf_adddef({
				"GF_SND_" .. string.upper(k)
			})
			break
		end
	end

	brk = false
	for k,v in pairs(gf_backends) do
		for k2,v2 in pairs(v["backends"]) do
			for k3,v3 in pairs(v["types"]) do
				if (_OPTIONS["backend"] == k) and (_OPTIONS[k] == k2) and (_OPTIONS[k .. "-type"] == k3) then
					gf_adddef({
						"GF_DRV_" .. string.upper(k),
						"GF_USE_" .. string.upper(v2.alias or k2),
						"GF_TYPE_" .. string.upper(k3)
					})
					brk = true
					break
				end
			end
			if brk then
				break
			end
		end
		if brk then
			break
		end
	end

	filter({})
end

function gf_generateheader(headerfile, placeholder, precstr)
	if os.isfile(headerfile) then
		return
	end
	local outfile = io.open(headerfile, "w")
	for i in io.lines(headerfile .. ".in") do
		local j,_ = string.gsub(i, placeholder, precstr)
		outfile:write(j .. "\n")
	end
	outfile:close()
end

function gf_link_stuffs(cond)
	filter({})
	filter({
		"platforms:Native",
		"system:not windows"
	})
		includedirs({
			"/usr/local/include",
			"/usr/X11R6/include",
			"/usr/X11R7/include",
			"/usr/X11/include"
		})
		libdirs({
			"/usr/local/lib",
			"/usr/X11R6/lib",
			"/usr/X11R7/lib",
			"/usr/X11/lib"
		})
	filter({
		"platforms:Native",
		"system:not windows",
		"toolset:gcc or toolset:clang"
	})
		linkoptions({
			"-Wl,-R/usr/local/lib",
			"-Wl,-R/usr/X11R6/lib",
			"-Wl,-R/usr/X11R7/lib",
			"-Wl,-R/usr/X11/lib"
		})

	filter({
		"platforms:Native",
		"system:bsd"
	})
		includedirs({
			"/usr/pkg/include"
		})
		libdirs({
			"/usr/pkg/lib"
		})
	filter({
		"platforms:Native",
		"system:bsd",
		"toolset:gcc or toolset:clang"
	})
		linkoptions({
			"-Wl,-R/usr/pkg/lib"
		})

	filter({
		"toolset:gcc or toolset:clang",
		"system:windows",
		cond
	})
		linkoptions({
			"-static-libgcc",
			"-static-libstdc++"
		})
		links({
			"stdc++:static"
		})
	filter({
		"toolset:gcc or toolset:clang",
		"system:not windows",
		cond
	})
		links({
			"stdc++"
		})
	for k,v in pairs(gf_sound_backends) do
		filter({
			"options:sound=" .. k
		})
			if v.links then
				links(v.links)
			end
	end
	for k,v in pairs(gf_sound_backends) do
		filter({
			"options:sound=" .. k,
			"platforms:Native"
		})
			if v.config then
				buildoptions("`" .. v.config .. "-config --cflags`")
				linkoptions("`" .. v.config .. "-config --libs`")
			end
	end
	for k,v in pairs(gf_backends) do
		for k2,v2 in pairs(v["backends"]) do
			for k3,v3 in pairs(v["types"]) do
				filter({
					"options:backend=" .. k,
					"options:" .. k .. "=" .. k2,
					"options:" .. k .. "-type=" .. k3,
					"system:windows",
					cond
				})
					if v2.links then
						links(v2.links)
					end
					if v2.windows then
						links(v2.windows)
					end
					if v3.links then
						links(v3.links)
					end
					if v3.windows then
						links(v3.windows)
					end
				filter({
					"options:backend=" .. k,
					"options:" .. k .. "=" .. k2,
					"options:" .. k .. "-type=" .. k3,
					"system:not windows",
					cond
				})
					if v2.links then
						links(v2.links)
					end
					if v2.unix then
						links(v2.unix)
					end
					if v3.links then
						links(v3.links)
					end
					if v3.unix then
						links(v3.unix)
					end
			end
		end
	end
	filter({
		"system:windows",
		cond
	})
		links({
			"user32",
			"wsock32",
			"winmm",
			"userenv"
		})
	if (_OPTIONS["ode"] == "system") and not(_OPTIONS["server"] == "no") then
		filter({
			cond
		})
			links({
				"ode"
			})
	end
	if _OPTIONS["zlib"] == "system" then
		filter({
			cond
		})
			links({
				"z"
			})
	end
	if _OPTIONS["pcre"] == "system" then
		filter({
			cond
		})
			links({
				"pcre"
			})
	end
	filter({
		"system:not windows",
		cond
	})
		links({
			"m",
		})
	if _OPTIONS["opengl"] ~= "agl" then
		links({
			"pthread"
		})
	end
	filter({})
end

function gf_msvc_filters()
	-- Manifest fix by penguin2233
	require('vstudio')
	premake.override(premake.vstudio.vc200x, "generateManifest", function(base, cfg, toolset)
		if cfg.flags.NoManifest then
			premake.w('GenerateManifest="false"')
		end
	end)
	filter({})
	characterset("MBCS")
	for k,rt in ipairs({"Debug", "Release"}) do
	filter({
			"options:cc=msc",
			"options:engine=dynamic",
			"configurations:" .. rt
		})
		runtime(rt)
		staticruntime("Off")
		buildoptions({
			"/wd4819",
			"/wd4996",
			"/wd4244",
			"/wd4789",
			"/wd4018",
			"/wd4305",
			"/wd4293",
			"/wd4552"
		})
	filter({
			"options:cc=msc",
			"options:engine=static",
			"configurations:" .. rt
		})
		runtime(rt)
		staticruntime("On")
		buildoptions({
			"/wd4819",
			"/wd4996",
			"/wd4244",
			"/wd4789",
			"/wd4018",
			"/wd4305",
			"/wd4293",
			"/wd4552"
		})
	end
	filter({})
end

if _OPTIONS["opengl"] ~= "agl" then
	include("util")
end
include("src")

if _ACTION and (_ACTION ~= "clean") and not(_OPTIONS["ode"] == "system") and not(_OPTIONS["server"] == "no") then
	local text = ""
	text = text .. "#ifndef _ODE_CONFIG_H_\n"
	text = text .. "#define _ODE_CONFIG_H_\n"
	text = text .. "#define dTRIMESH_ENABLED 1\n"
	text = text .. "#define dTRIMESH_GIMPACT 1\n"
	if _OPTIONS["opengl"] ~= "agl" then
		text = text .. "#define dOU_ENABLED 1\n"
		text = text .. "#define dATOMICS_ENABLED 1\n"
		text = text .. "#define dTLS_ENABLED 1\n"
		text = text .. "#define dBUILTIN_THREADING_IMPL_ENABLED 1\n"
	else
		text = text .. "#define dOU_ENABLED 1\n"
		text = text .. "#define dATOMICS_ENABLED 0\n"
		text = text .. "#define dTLS_ENABLED 0\n"
		text = text .. "#define dBUILTIN_THREADING_IMPL_ENABLED 0\n"
		text = text .. "#define dTHREADING_INTF_DISABLED 1\n"
	end
	text = text .. "#include \"typedefs.h\"\n"
	text = text .. "#endif\n"

	if not(os.isfile("external/ode/ode/src/config.h")) then
		local outfile = io.open("external/ode/ode/src/config.h", "w")
		outfile:write(text)
		outfile:close()
	end

	gf_generateheader("external/ode/include/ode/precision.h", "@ODE_PRECISION@", "dDOUBLE")
	gf_generateheader("external/ode/libccd/src/ccd/precision.h", "@CCD_PRECISION@", "CCD_DOUBLE")
	gf_generateheader("external/ode/include/ode/version.h", "@ODE_VERSION@", "Custom-ODE")
end

local outfile = io.open("include/gf_config.h", "w")
outfile:write("#ifndef __GF_CONFIG_H__\n")
outfile:write("#define __GF_CONFIG_H__\n")
if _OPTIONS["engine"] == "dynamic" then
	outfile:write("#define GF_BUILT_AS_DLL 1\n")
end
if _OPTIONS["server"] == "no" then
	outfile:write("#define GF_NO_SERVER 1\n")
end
outfile:write("#ifdef _WIN32\n")
outfile:write("#define GF_THREAD_WIN32 1\n")
outfile:write("#else\n")
outfile:write("#define GF_THREAD_POSIX 1\n")
outfile:write("#endif\n")
for _,v in ipairs(gf_defs) do
	outfile:write("#define " .. v .. " 1\n")
end
outfile:write("#endif\n")
outfile:close()

if (_OPTIONS and _OPTIONS["opengl"] == "rgfw-wayland") and (_ACTION and _ACTION ~= "clean") then
        os.execute("wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml external/rgfw/xdg-shell.h")
        os.execute("wayland-scanner public-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml external/rgfw/xdg-shell.c")
        os.execute("wayland-scanner client-header /usr/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml external/rgfw/xdg-decoration-unstable-v1.h")
        os.execute("wayland-scanner public-code /usr/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml external/rgfw/xdg-decoration-unstable-v1.c")
        os.execute("wayland-scanner client-header /usr/share/wayland-protocols/unstable/relative-pointer/relative-pointer-unstable-v1.xml external/rgfw/relative-pointer-unstable-v1-client-protocol.h")
        os.execute("wayland-scanner client-header /usr/share/wayland-protocols/unstable/relative-pointer/relative-pointer-unstable-v1.xml external/rgfw/relative-pointer-unstable-v1-client-protocol.c")
end
