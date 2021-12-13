workspace "Binary Diagnostic"
    language "C++"
    cppdialect "C++17"
    configurations {"Debug", "Release"}
    platforms {"LinuxX64", "LinuxX86", "LinuxARM", "WindowsARM", "WindowsX86", "WindowsX64"}
    startproject "Binary Diagnostic"

    flags {"MultiProcessorCompile"}
    buildoptions {"-pipe", "-Wall"}
    defines {}

    links {}
    includedirs {"%{wks.location}/include/"}

    filter "action:vs*"
        toolset "msc"
        buildoptions {"/W4"}

    filter "action:not vs*"
        toolset "gcc"
        buildoptions {"-Wpedantic", "-Wextra", "-Wnon-virtual-dtor", "-Winline", "-Wunreachable-code", "-Wshadow", "-Wconversion", "-Wno-switch", "-Wno-unused-variable", "-Wold-style-cast",
        	      "-Wcast-align", "-Wunused", "-Woverloaded-virtual", "-Wsign-conversion", "-Wduplicated-cond", "-Wduplicated-branches", "-Wlogical-op", "-Wnull-dereference", "-Wuseless-cast",
        	      "-Wdouble-promotion"}

    filter "platforms:Linux*"
        system "linux"
    filter "platforms:Windows*"
        system "windows"

    filter "platforms:*ARM"
        architecture "ARM"
    filter "platforms:*X86"
        architecture "x86"
    filter "platforms:*X64"
        architecture "x86_64"

    filter "configurations:Debug"
        symbols "On"
        links {"ubsan"}
        buildoptions {"-pg", "-fsanitize=address", "-fsanitize=leak", "-fsanitize=undefined", "-static-libasan"}
        linkoptions {"-pg", "-fsanitize=address", "-fsanitize=leak", "-fsanitize=undefined", "-static-libasan"}

    filter "configurations:Release"
        optimize "Speed"
        flags {"LinkTimeOptimization"}

    project "Binary Diagnostic"
        kind "ConsoleApp"
        location "%{wks.location}"
        targetdir "%{prj.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

        files {"%{prj.location}/src/**.cpp", "%{prj.location}/include/**.hpp", "%{prj.location}/include/**.h", "%{prj.location}/premake5.lua"}