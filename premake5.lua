---@diagnostic disable: undefined-global, undefined-field
require"vendor/river2D/premake5"

workspace("mapedit")
    configurations({"debug", "asan", "release"})
    platforms({"linux", "windows"})
    location("build")
    architecture("x86_64")

project("mapedit binary")
    language("C")
    cdialect("C99")
    warnings("Extra")
    targetname("mapedit")
    libdirs({"./vendor/river2D/bin/%{cfg.buildcfg}/", "./vendor/river2D/vendor/imgsurf/bin/%{cfg.buildcfg}/"})
    includedirs({"./include/", "/usr/include/", "./vendor/river2D/include/", "./vendor/river2D/vendor/imgsurf/include"})
    debugdir("./")

    filter("configurations:asan")
        defines{"ASAN"}

    filter("configurations:debug")
        defines{"DEBUG"}

    filter("configurations:debug or asan")
        runtime("debug")
        symbols("On")
        optimize("Off")

    filter("configurations:release")
        defines{"NDEBUG"}
        staticruntime("off")
        runtime("release")
        symbols("Off")
        optimize("Speed")

    filter("platforms:Linux")
        system("Linux")
        defines("BUILD_LINUX")
        kind("ConsoleApp")
        targetdir("bin/%{cfg.buildcfg}")
        objdir("obj/%{cfg.buildcfg}")
        files({"./src/linux_mapedit*",
               "./include/linux_mapedit*",
               "./src/mapedit_*",
               "./include/mapedit_*"})
        links({"river2Dcommon:static", "imgsurf:static"})
        linkoptions({"-lX11", "-lXrender", "-lXcursor", "-lm", "-fuse-ld=mold"})
        buildoptions({"-Wextra", "-Wall", "-Wpedantic", "-Wconversion", "-Wshadow", "-Wsign-compare"})
        toolset("clang")

    filter("platforms:Windows")
        system("Windows")
        defines("BUILD_WINDOWS")
        targetdir("bin/%{cfg.buildcfg}")
        objdir("obj/")
        files({"./src/win32_mapedit*",
               "./include/win32_mapedit*",
               "./src/mapedit_*",
               "./include/mapedit_*"})
        links({"river2Dcommon.lib", "imgsurf.lib"})
        buildoptions{"/wd4068", "/wd4100"}

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
        buildoptions({"/fsanitize=address", "/Zi", "/INCREMENTAL:NO"})

    filter({"platforms:Windows", "configurations:release"})
        kind("WindowedApp")
        linkoptions("/NODEFAULTLIB:MSVCRTD")
