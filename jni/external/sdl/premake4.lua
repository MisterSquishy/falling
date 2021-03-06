project "local_SDL"
  kind "SharedLib"
  language "C"

  configuration "windows"
    files {
            "src/joystick/win32/*.c",
--            "src/cdrom/win32/*.c",
            "src/loadso/win32/*.c",
            "src/thread/generic/SDL_syscond.c",
            "src/thread/win32/*.c",
            "src/timer/win32/*.c",
            "src/video/wincommon/*.c",
            "src/video/windib/*.c",
            "src/video/windx5/*.c",
    }
    links { "winmm", "dxguid" }
  configuration { "windows", "x32" }
    libdirs { "../lib/x32" }
  configuration { "windows", "x64" }
    libdirs { "../lib/x64" }
  configuration "linux"
    defines { "_GNU_SOURCE=1",
              "_REENTRANT",
              "XTHREADS",
              "HAVE_LINUX_VERSION_H",
              "HAVE_ALLOCA_H=1",
              "HAVE_SYS_TYPES_H=1",
              "HAVE_STDIO_H=1",
              "STDC_HEADERS=1",
              "HAVE_STDLIB_H=1",
              "HAVE_STDARG_H=1",
              "HAVE_MALLOC_H=1",
              "HAVE_MEMORY_H=1",
              "HAVE_STRING_H=1",
              "HAVE_STRINGS_H=1",
              "HAVE_INTTYPES_H=1",
              "HAVE_STDINT_H=1",
              "HAVE_CTYPE_H=1",
              "HAVE_MATH_H=1",
              "HAVE_ICONV_H=1",
              "HAVE_SIGNAL_H=1",
              "HAVE_MALLOC=1",
              "HAVE_CALLOC=1",
              "HAVE_REALLOC=1",
              "HAVE_FREE=1",
              "HAVE_ALLOCA=1",
              "HAVE_QSORT=1",
              "HAVE_ABS=1",
              "HAVE_BCOPY=1",
              "HAVE_MEMSET=1",
              "HAVE_MEMCPY=1",
              "HAVE_MEMMOVE=1",
              "HAVE_MEMCMP=1",
              "HAVE_STRLEN=1",
              "HAVE_STRDUP=1",
              "HAVE_STRCHR=1",
              "HAVE_STRRCHR=1",
              "HAVE_STRSTR=1",
              "HAVE_STRTOL=1",
              "HAVE_STRTOUL=1",
              "HAVE_STRTOLL=1",
              "HAVE_STRTOULL=1",
              "HAVE_STRTOD=1",
              "HAVE_ATOI=1",
              "HAVE_ATOF=1",
              "HAVE_STRCMP=1",
              "HAVE_STRNCMP=1",
              "HAVE_STRCASECMP=1",
              "HAVE_STRNCASECMP=1",
              "HAVE_SSCANF=1",
              "HAVE_SNPRINTF=1",
              "HAVE_VSNPRINTF=1",
              "HAVE_ICONV=1",
              "HAVE_SIGACTION=1",
              "HAVE_SA_SIGACTION=1",
              "HAVE_SETJMP=1",
              "HAVE_NANOSLEEP=1",
              "HAVE_GETPAGESIZE=1",
              "HAVE_MPROTECT=1",
              "HAVE_SEM_TIMEDWAIT=1",
              "SDL_AUDIO_DRIVER_DUMMY=1",
              "SDL_CDROM_LINUX=1",
              "SDL_INPUT_LINUXEV=1",
              "SDL_JOYSTICK_LINUX=1",
              "SDL_LOADSO_DLOPEN=1",
              "SDL_THREAD_PTHREAD=1",
              "SDL_THREAD_PTHREAD_RECURSIVE_MUTEX=1",
              "SDL_TIMER_UNIX=1",
--               "SDL_VIDEO_DRIVER_DGA=1",
              "SDL_VIDEO_DRIVER_DUMMY=1",
--               "SDL_VIDEO_DRIVER_FBCON=1",
              "SDL_VIDEO_DRIVER_X11=1",
--               "SDL_VIDEO_DRIVER_X11_DGAMOUSE=1",
--               "SDL_VIDEO_DRIVER_X11_VIDMODE=1",
--               "SDL_VIDEO_DRIVER_X11_XINERAMA=1",
--               "SDL_VIDEO_DRIVER_X11_XME=1",
--               "SDL_VIDEO_DRIVER_X11_XV=1",
              "SDL_VIDEO_OPENGL=1",
              "SDL_VIDEO_OPENGL_GLX=1",
              "SDL_VIDEO_DISABLE_SCREENSAVER=1",
              "SDL_ASSEMBLY_ROUTINES=1" }
    files { "src/loadso/dlopen/*.c",
            "src/video/x11/*.c",
  --           "src/video/Xext/Xxf86dga/*.c",
  --           "src/video/dga/*.c",
  --           "src/video/Xext/Xxf86vm/*.c",
  --           "src/video/Xext/Xv/*.c",
  --           "src/video/Xext/Xinerama/*.c",
  --           "src/video/Xext/XME/*.c",
  --           "src/video/fbcon/*.c",
            "src/thread/pthread/*.c",
            "src/joystick/linux/*.c",
  --           "src/cdrom/linux/*.c",
            "src/timer/unix/*.c" }
    links { "m", "dl", "pthread", "X11", "Xext" }
  configuration "macosx"
    buildoptions { "-fvisibility=hidden",
                   "-I/usr/X11R6/include",
                   "-force_cpusubtype_ALL",
                   "-fpascal-strings",
                   "-Wall" }
    defines { "SDL_HAS_64BIT_TYPE=1",
              "SDL_BYTEORDER=1234",
              "HAVE_ALLOCA_H=1",
              "HAVE_SYS_TYPES_H=1",
              "HAVE_STDIO_H=1",
              "STDC_HEADERS=1",
              "HAVE_STDLIB_H=1",
              "HAVE_STDARG_H=1",
              "HAVE_MEMORY_H=1",
              "HAVE_STRING_H=1",
              "HAVE_STRINGS_H=1",
              "HAVE_INTTYPES_H=1",
              "HAVE_STDINT_H=1",
              "HAVE_CTYPE_H=1",
              "HAVE_MATH_H=1",
              "HAVE_ICONV_H=1",
              "HAVE_SIGNAL_H=1",
              "HAVE_MALLOC=1",
              "HAVE_CALLOC=1",
              "HAVE_REALLOC=1",
              "HAVE_FREE=1",
              "HAVE_ALLOCA=1",
              "HAVE_GETENV=1",
              "HAVE_PUTENV=1",
              "HAVE_UNSETENV=1",
              "HAVE_QSORT=1",
              "HAVE_ABS=1",
              "HAVE_BCOPY=1",
              "HAVE_MEMSET=1",
              "HAVE_MEMCPY=1",
              "HAVE_MEMMOVE=1",
              "HAVE_MEMCMP=1",
              "HAVE_STRLEN=1",
              "HAVE_STRLCPY=1",
              "HAVE_STRLCAT=1",
              "HAVE_STRDUP=1",
              "HAVE_STRCHR=1",
              "HAVE_STRRCHR=1",
              "HAVE_STRSTR=1",
              "HAVE_STRTOL=1",
              "HAVE_STRTOUL=1",
              "HAVE_STRTOLL=1",
              "HAVE_STRTOULL=1",
              "HAVE_STRTOD=1",
              "HAVE_ATOI=1",
              "HAVE_ATOF=1",
              "HAVE_STRCMP=1",
              "HAVE_STRNCMP=1",
              "HAVE_STRCASECMP=1",
              "HAVE_STRNCASECMP=1",
              "HAVE_SSCANF=1",
              "HAVE_SNPRINTF=1",
              "HAVE_VSNPRINTF=1",
              "HAVE_SIGACTION=1",
              "HAVE_SA_SIGACTION=1",
              "HAVE_SETJMP=1",
              "HAVE_NANOSLEEP=1",
              "HAVE_MPROTECT=1",
              "SDL_AUDIO_DRIVER_COREAUDIO=1",
              "SDL_AUDIO_DRIVER_DISK=1",
              "SDL_AUDIO_DRIVER_DUMMY=1",
              "SDL_CDROM_DISABLED",
  --            "SDL_CDROM_MACOSX=1",
              "SDL_JOYSTICK_IOKIT=1",
              "SDL_LOADSO_DLOPEN=1",
              "SDL_THREAD_PTHREAD=1",
              "SDL_THREAD_PTHREAD_RECURSIVE_MUTEX=1",
              "SDL_TIMER_UNIX=1",
--              "SDL_VIDEO_DRIVER_DGA=1",
              "SDL_VIDEO_DRIVER_DUMMY=1",
              "SDL_VIDEO_DRIVER_QUARTZ=1",
--              "SDL_VIDEO_DRIVER_X11=1",
--              "SDL_VIDEO_DRIVER_X11_DGAMOUSE=1",
--              "SDL_VIDEO_DRIVER_X11_DYNAMIC=\\\"/usr/X11R6/lib/libX11.6.dylib\\\"",
--              "SDL_VIDEO_DRIVER_X11_DYNAMIC_XEXT=\\\"/usr/X11R6/lib/libXext.6.dylib\\\"",
--              "SDL_VIDEO_DRIVER_X11_DYNAMIC_XRANDR=\\\"/usr/X11R6/lib/libXrandr.2.dylib\\\"",
--              "SDL_VIDEO_DRIVER_X11_DYNAMIC_XRENDER=\\\"/usr/X11R6/lib/libXrender.1.dylib\\\"",
--              "SDL_VIDEO_DRIVER_X11_VIDMODE=1",
--              "SDL_VIDEO_DRIVER_X11_XINERAMA=1",
--              "SDL_VIDEO_DRIVER_X11_XME=1",
--              "SDL_VIDEO_DRIVER_X11_XRANDR=1",
--              "SDL_VIDEO_DRIVER_X11_XV=1",
              "SDL_VIDEO_OPENGL=1",
              "SDL_VIDEO_OPENGL_GLX=1",
              "SDL_VIDEO_DISABLE_SCREENSAVER=1",
              "SDL_ASSEMBLY_ROUTINES=1" }
    includedirs { "/usr/X11R6/include" }
    files { "src/audio/disk/*.c",
            "src/loadso/dlopen/*.c",
            "src/video/quartz/*.m",
--            "src/video/x11/*.c",
--            "src/video/Xext/Xxf86dga/*.c",
--            "src/video/dga/*.c",
--            "src/video/Xext/Xxf86vm/*.c",
--            "src/video/Xext/Xv/*.c",
--            "src/video/Xext/Xinerama/*.c",
--            "src/video/Xext/XME/*.c",
            "src/thread/pthread/*.c",
            "src/audio/macosx/*.c",
            "src/joystick/darwin/*.c",
            "src/timer/unix/*.c" }
    links { "ApplicationServices.framework",
            "AudioUnit.framework",
            "Carbon.framework",
            "Cocoa.framework",
            "IOKit.framework",
            "OpenGL.framework" }
  configuration { "macosx", "Debug*" }
    linkoptions { "-install_name @loader_path/liblocal_SDL_d.dylib" }
    targetdir "../../../lib/univ_d"
  configuration { "macosx", "Release*" }
    linkoptions { "-install_name @loader_path/liblocal_SDL.dylib" }
    targetdir "../../../lib/univ"

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { "SDL" }
    files { "include/**.h",
            "src/*.c",
            "src/audio/*.c",
  --           "src/cdrom/*.c",
            "src/cpuinfo/*.c",
            "src/events/*.c",
            "src/file/*.c",
            "src/stdlib/*.c",
            "src/thread/*.c",
            "src/timer/*.c",
            "src/video/*.c",
            "src/joystick/*.c",
            "src/video/dummy/*.c",
            "src/audio/dummy/*.c" }

if os.get() ~= "macosx" then
  project "local_SDLmain"
    kind "StaticLib"
    language "C"

    configuration "windows"
      files { "src/main/win32/*.c" }
    configuration "linux"
      defines { "_GNU_SOURCE=1",
                "_REENTRANT",
                "XTHREADS",
                "HAVE_LINUX_VERSION_H",
                "HAVE_ALLOCA_H=1",
                "HAVE_SYS_TYPES_H=1",
                "HAVE_STDIO_H=1",
                "STDC_HEADERS=1",
                "HAVE_STDLIB_H=1",
                "HAVE_STDARG_H=1",
                "HAVE_MALLOC_H=1",
                "HAVE_MEMORY_H=1",
                "HAVE_STRING_H=1",
                "HAVE_STRINGS_H=1",
                "HAVE_INTTYPES_H=1",
                "HAVE_STDINT_H=1",
                "HAVE_CTYPE_H=1",
                "HAVE_MATH_H=1",
                "HAVE_ICONV_H=1",
                "HAVE_SIGNAL_H=1",
                "HAVE_MALLOC=1",
                "HAVE_CALLOC=1",
                "HAVE_REALLOC=1",
                "HAVE_FREE=1",
                "HAVE_ALLOCA=1",
                "HAVE_QSORT=1",
                "HAVE_ABS=1",
                "HAVE_BCOPY=1",
                "HAVE_MEMSET=1",
                "HAVE_MEMCPY=1",
                "HAVE_MEMMOVE=1",
                "HAVE_MEMCMP=1",
                "HAVE_STRLEN=1",
                "HAVE_STRDUP=1",
                "HAVE_STRCHR=1",
                "HAVE_STRRCHR=1",
                "HAVE_STRSTR=1",
                "HAVE_STRTOL=1",
                "HAVE_STRTOUL=1",
                "HAVE_STRTOLL=1",
                "HAVE_STRTOULL=1",
                "HAVE_STRTOD=1",
                "HAVE_ATOI=1",
                "HAVE_ATOF=1",
                "HAVE_STRCMP=1",
                "HAVE_STRNCMP=1",
                "HAVE_STRCASECMP=1",
                "HAVE_STRNCASECMP=1",
                "HAVE_SSCANF=1",
                "HAVE_SNPRINTF=1",
                "HAVE_VSNPRINTF=1",
                "HAVE_ICONV=1",
                "HAVE_SIGACTION=1",
                "HAVE_SA_SIGACTION=1",
                "HAVE_SETJMP=1",
                "HAVE_NANOSLEEP=1",
                "HAVE_GETPAGESIZE=1",
                "HAVE_MPROTECT=1",
                "HAVE_SEM_TIMEDWAIT=1",
                "SDL_AUDIO_DRIVER_DUMMY=1",
                "SDL_CDROM_LINUX=1",
                "SDL_INPUT_LINUXEV=1",
                "SDL_JOYSTICK_LINUX=1",
                "SDL_LOADSO_DLOPEN=1",
                "SDL_THREAD_PTHREAD=1",
                "SDL_THREAD_PTHREAD_RECURSIVE_MUTEX=1",
                "SDL_TIMER_UNIX=1",
--                 "SDL_VIDEO_DRIVER_DGA=1",
                "SDL_VIDEO_DRIVER_DUMMY=1",
--                 "SDL_VIDEO_DRIVER_FBCON=1",
                "SDL_VIDEO_DRIVER_X11=1",
--                 "SDL_VIDEO_DRIVER_X11_DGAMOUSE=1",
--                 "SDL_VIDEO_DRIVER_X11_VIDMODE=1",
--                 "SDL_VIDEO_DRIVER_X11_XINERAMA=1",
--                 "SDL_VIDEO_DRIVER_X11_XME=1",
--                 "SDL_VIDEO_DRIVER_X11_XV=1",
                "SDL_VIDEO_OPENGL=1",
                "SDL_VIDEO_OPENGL_GLX=1",
                "SDL_VIDEO_DISABLE_SCREENSAVER=1",
                "SDL_ASSEMBLY_ROUTINES=1" }
      files { "src/main/dummy/*.c" }

  configuration "not windows"
    buildoptions { "-w" }

  configuration "*"
    includedirs { ".", "SDL" }
end
