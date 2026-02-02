---@diagnostic disable: undefined-global, undefined-field
require"vendor/premake-ecc/ecc"

workspace("mapedit")
    configurations({"debug", "asan", "release"})
    platforms({"linux", "windows"})
    location("build")
    architecture("x86_64")

project("mapedit binary")
    language("C")
    cdialect("C99")
    warnings("Extra")
    targetname("river2Dmapedit")
    libdirs({"./bin/%{cfg.buildcfg}/", "./vendor/river2D/vendor/imgsurf/bin/%{cfg.buildcfg}/"})
    includedirs({"./include/", "/usr/include/", "./vendor/imgsurf/include/"})
    debugdir("./")

    filter("configurations:debug or asan")
        defines{"DEBUG"}
        runtime("debug")
        symbols("On")
        optimize("Off")

    filter("configurations:release")
        staticruntime("off")
        runtime("release")
        symbols("Off")
        optimize("Speed")

    filter("platforms:Linux")
        system("Linux")
        defines("BUILD_LINUX")
        kind("ConsoleApp")
        targetdir("bin/%{cfg.buildcfg}")
        objdir("obj/river2Dmapedit/")
        files({"./src/linux_river2Dmapedit*",
               "./include/linux_river2Dmapedit*",
               "./src/river2Dmapedit*",
               "./include/river2Dmapedit*" })
        links("river2Dcommon:static", "imgsurf:static")
        linkoptions({"-lX11", "-fuse-ld=mold"})
        buildoptions({"-Wextra", "-Wall", "-Wpedantic"})
        toolset("clang")

    filter("platforms:Windows")
        system("Windows")
        defines("BUILD_WINDOWS")
        targetdir("bin/%{cfg.buildcfg}")
        objdir("obj/")
        files({ "./src/win32_river2Dmapedit*",
                "./include/win32_river2Dmapedit*",
                "./src/river2Dmapedit*",
                "./include/river2Dmapedit*" })
        libdirs({"./vendor/imgsurf/bin/%{cfg.buildcfg}/"})
        links({"river2Dcommon.lib", "imgsurf.lib"})
        buildoptions({"/wd4068"})

    filter({"platforms:Linux", "configurations:debug or asan"})
        buildoptions({"-gfull", "-O1"})
        linkoptions({"-gfull", "-O1"})

    filter({"platforms:Linux", "configurations:asan"})
        buildoptions({"-fsanitize=address,leak,undefined", "-fno-omit-frame-pointer",
                      "-static-libasan"})
        linkoptions({"-fsanitize=address,leak,undefined", "-fno-omit-frame-pointer",
                     "-static-libasan"})

    filter({"platforms:Windows", "configurations:debug or asan"})
        kind("ConsoleApp")

    filter({"platforms:Windows", "configurations:asan"})
        editandcontinue("Off")
        debugformat("c7")
        buildoptions({"/fsanitize=address"})

    filter({"platforms:Windows", "configurations:release"})
        kind("WindowedApp")
        linkoptions("/NODEFAULTLIB:MSVCRTD")
