project("GoldFish")
	filter("options:engine=static")
		kind("StaticLib")
		defines({
			"ODE_LIB",
			"GF_LIB"
		})
	filter("options:engine=dynamic")
		kind("SharedLib")
		defines({
			"ODE_DLL",
			"GF_DLL"
		})
	filter("configurations:Debug")
		defines({
			"DEBUG",
			"_DEBUG"
		})
		symbols("On")
	filter("configurations:Release")
		defines({
			"NDEBUG",
			"dNODEBUG"
		})
		optimize("On")
	gf_msvc_filters()
	targetdir("../lib/%{cfg.buildcfg}/%{cfg.platform}")
	objdir("../obj")
	targetname("goldfish")
	includedirs({
		"../include",
		"../external/lua",
		"../external/zlib",
		"../external/miniaudio",
		"../external/stb",
		"../external/jar",
		"bindgen"
	})
	files({
		"../include/**.h",
		"*.c",
		"../external/zlib/*.h",
		"../external/zlib/*.c",
		"../external/lua/l*.h",
		"../external/lua/l*.c",
		"bindgen/*.c",
		"bindgen/*.h"
	})
	files({
		"audio/*.c",
		"../external/jar/jar_*.h"
	})
	files({
		"gui/*.c"
	})
	filter({
		"system:windows",
		"options:engine=dynamic"
	})
		files({
			"engine.rc"
		})
	filter({})
	removefiles({
		"../external/lua/ltests.c",
		"../external/lua/ltests.h"
	})
	removefiles({
		"../external/lua/lua.c"
	})

	filter("system:windows")
		files({
			"thread/win32/gf_thread.c"
		})
	filter("system:not windows")
		files({
			"thread/posix/gf_thread.c"
		})
	filter({})

	-- Begin ODE
	includedirs({
		"../external/ode/include",
		"../external/ode/ode/src",
		"../external/ode/ode/src/joints",
		"../external/ode/OPCODE",
		"../external/ode/GIMPACT/include",
		"../external/ode/libccd/src/custom",
		"../external/ode/libccd/src"
	})
	files({
		"../external/ode/include/ode/*.h",
		"../external/ode/ode/src/joints/*.h",
		"../external/ode/ode/src/joints/*.cpp",
		"../external/ode/ode/src/*.h",
		"../external/ode/ode/src/*.c",
		"../external/ode/ode/src/*.cpp"
	})
	removefiles({
		"../external/ode/ode/src/collision_trimesh_trimesh_old.cpp",
		"../external/ode/ode/src/collision_trimesh_opcode.cpp",
		"../external/ode/ode/src/collision_trimesh_disabled.cpp"
	})

	includedirs({
		"../external/ode/ou/include"
	})
	files({
		"../external/ode/ou/include/**.h",
		"../external/ode/ou/src/**.h",
		"../external/ode/ou/src/**.cpp"
	})
	defines({
		"_OU_NAMESPACE=odeou",
		"_OU_FEATURE_SET=_OU_FEATURE_SET_TLS"
	})

	files({
		"../external/ode/GIMPACT/**.h",
		"../external/ode/GIMPACT/**.cpp"
	})

	files({
		"../external/ode/libccd/src/custom/ccdcustom/*.h",
		"../external/ode/libccd/src/ccd/*.h",
		"../external/ode/libccd/src/*.c"
	})

	defines({
		"dLIBCCD_ENABLED",
		"dLIBCCD_INTERNAL",
		"dLIBCCD_BOX_CYL",
		"dLIBCCD_CYL_CYL",
		"dLIBCCD_CAP_CYL",
		"dLIBCCD_CONVEX_BOX",
		"dLIBCCD_CONVEX_CAP",
		"dLIBCCD_CONVEX_CYL",
		"dLIBCCD_CONVEX_SPHERE",
		"dLIBCCD_CONVEX_CONVEX"
	})
	-- End ODE
	
	for k,v in pairs(gf_backends) do
		for k2,v2 in pairs(v["backends"]) do
			for k3,v3 in pairs(v["types"]) do
				filter({
					"options:backend=" .. k,
					"options:" .. k .. "=" .. k2,
					"options:" .. k .. "-type=" .. k3
				})
					if v2.includedirs then
						local l = {}
						for _,d in ipairs(v2.includedirs) do
							table.insert(l, "../" .. d)
						end
						includedirs(l)
					end
					if v3.includedirs then
						local l = {}
						for _,d in ipairs(v3.includedirs) do
							table.insert(l, "../" .. d)
						end
						includedirs(l)
					end
					if v2.defines then
						defines(v2.defines)
					end
					if v3.defines then
						defines(v3.defines)
					end
					if v2.files then
						for _,fn in ipairs(v2.files) do
							if string.sub(fn, 1, 1) == "-" then
								removefiles("../" .. string.sub(fn, 2, -1))
							else
								files("../" .. fn)
							end
						end
					end
					if v3.files then
						for _,fn in ipairs(v3.files) do
							if string.sub(fn, 1, 1) == "-" then
								removefiles("../" .. string.sub(fn, 2, -1))
							else
								files("../" .. fn)
							end
						end
					end
			end
		end
	end
	filter({})

	gf_default_stuffs()
	gf_link_stuffs("options:engine=dynamic")
	for k,v in pairs(gf_backends) do
		for k2,v2 in pairs(v["backends"]) do
			filter({
				"options:backend=" .. k,
				"options:" .. k .. "=" .. k2
			})
				files({
					"graphic/" .. k .. "/*.c",
					"graphic/" .. k .. "/" .. (v2.alias or k2) .. "/*.c"
				})
		end
	end
