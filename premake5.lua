workspace "LearnOpengl"
    configurations {"Debug", "Release"}
    architecture "x64"

project "LearnOpenGL"
    kind "ConsoleApp"
    language "C++"
    targetdir "build/"
    targetname "LearnOpenGL"

    files {"vendor/GLAD/src/glad.c","src/**.h", "src/**.cpp"}

    includedirs {
        "vendor/GLAD/include",
        "vendor/glfw/include",
        "vendor/assimp/include",
        "vendor/assimp/build/include",
        "vendor/glm",
        "vendor/stb"
    }

    libdirs{
        "vendor/glfw",
        "vendor/assimp/build/lib/Debug"
    }

    filter "system:windows"
        links{
            "glfw3",
            "assimp-vc143-mtd",
            
        }


    filter "system:linux"
        links{
            "glfw3",
            "assimp",
            "GL",
            "dl",
            "pthread",
            "X11",
            "Xxf86vm",
            "Xrandr",
            "Xi",
            "z"
        }

    filter "configurations:Debug"
        defines{"DEBUG"}
        symbols "On"
    filter "configurations:Release"
        defines{"NDEBUG"}
        optimize "On"
    
