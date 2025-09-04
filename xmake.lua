set_project("laurel")
set_version("0.1.0")

set_arch("x64")
set_plat("windows")
set_toolchains("msvc")
set_languages("c++20")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("plugin.vsxmake.autoupdate")

if is_mode("debug") then
    add_defines("LDEBUG")
end

if is_plat("windows") then
    add_defines("LR_PLATFORM_WINDOWS")
end

local include_dir = {
    "source",
    "external/include"
}

local link_dirs = {
    "external/lib/GLFW"
}

local external_links = {
    "glfw3"
}

local system_links = {
    "user32", 
    "gdi32", 
    "shell32"
}

target("laurel")
    set_kind("static")

    add_includedirs(include_dir)

    add_linkdirs(link_dirs)
    add_links(external_links)
    add_syslinks(system_links)

    add_files(
        "source/laurel/**.cpp",
        "source/volk/volk.c"
    )

    add_headerfiles(
        "source/**.hpp",
        "source/**.h"
    )
target_end()