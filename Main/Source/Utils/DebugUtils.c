// (C) Sebastian Fiault 2026
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Constants.h"
#include "../GalaxyCollision.h"
#include "DataUtils.h"

// Quit the program as if an error happened
void quitProgramOnError( 
    void* pointersToFree[], //TODO abstract these parameters into a struct
    size_t amountPointers,
    GLuint buffersToDelete[],
    size_t amountBuffers,
    GLuint vertexArraysToDelete[],
    size_t amountVertexArrays,
    GLuint shaderProgramsToDelete[],
    size_t amountShaderPrograms,
    const char* extraInfo
)
{
    const char* stringToPrint = "Something went wrong\n";

    if (extraInfo) 
    {
        perror(formatString("%s %s; %s\n",ERROR_TAG, stringToPrint, extraInfo));
    }
    else
    {
        perror(stringToPrint);
    }

    if (pointersToFree && amountPointers)
    {
        for (size_t i = 0; i < amountPointers; i++)
        {
            if (pointersToFree[i]) safer_free(&pointersToFree[i]);
        }
    }
    else if (pointersToFree)
    {
        perror( formatString("%s You specified \"pointersToFree\" but forgot to specify the amount \"amountPointers\" (DebugUtils.c quitProgram())\n", ERROR_TAG) );
    }

    if (buffersToDelete && amountBuffers)
    {
        glDeleteBuffers(amountBuffers, buffersToDelete);
    }
    else if (buffersToDelete)
    {
        perror( formatString("%s You specified \"buffersToDelete\" but forgot to specify the amount \"amountBuffers\" (DebugUtils.c quitProgram())\n", ERROR_TAG));
    }

    if (vertexArraysToDelete && amountVertexArrays)
    {
        glDeleteVertexArrays(amountVertexArrays, vertexArraysToDelete);
    }
    else if (vertexArraysToDelete)
    {
        perror(formatString("%s You specified \"vertexArraysToDelete\" but forgot to specify the amount \"amountVertexArrays\" (DebugUtils.c quitProgram())\n", ERROR_TAG));
    }

    if (shaderProgramsToDelete && amountShaderPrograms)
    {
        for (size_t i = 0; i < amountShaderPrograms; i++)
        {
            glDeleteProgram(shaderProgramsToDelete[i]);
        }
    }
    else if (shaderProgramsToDelete)
    {
        perror(formatString("%s You specified \"shaderProgramsToDelete\" but forgot to specify the amount \"amountShaderPrograms\" (DebugUtils.c quitProgram())\n", ERROR_TAG));
    }

    glfwTerminate();
    exit(EXIT_FAILURE);
}

//TODO void quitProgram() {} // Like quitProgramOnError only for use when there are no errors

const char* getCompiler()
{
    const char* result;
    #if defined(__clang__)
        result = formatString("Clang %s", __clang_version__);
    #elif defined(__ICC) || defined(__INTEL_COMPILER)
        result = formatString("Intel C/C++ compiler version %d", __INTEL_COMPILER);
    #elif defined(__GNUC__)
        result = formatString("GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #elif defined(_MSC_VER)
        result = formatString("MSVC version %d", _MSC_VER);
    #else
        result = formatString("Unknown compiler");
    #endif

    return result;
}

// Source - https://stackoverflow.com/a/66249936
// Posted by FreakAnon, modified by community. See post 'Timeline' for change history
// Retrieved 2026-06-23, License - CC BY-SA 4.0
// Slightly modified by gamagos a.k.a. Sebastian Fiault
const char* getBuildPlatform() // Get current architecture, detects nearly every architecture. Coded by Freak
{ 
    #if defined(__x86_64__) || defined(_M_X64)
        return "x86_64";
    #elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
        return "x86_32";
    #elif defined(__ARM_ARCH_2__)
        return "ARM2";
    #elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
        return "ARM3";
    #elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
        return "ARM4T";
    #elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
        return "ARM5"
    #elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
        return "ARM6T2";
    #elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
        return "ARM6";
    #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7";
    #elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7A";
    #elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7R";
    #elif defined(__ARM_ARCH_7M__)
        return "ARM7M";
    #elif defined(__ARM_ARCH_7S__)
        return "ARM7S";
    #elif defined(__aarch64__) || defined(_M_ARM64)
        return "ARM64";
    #elif defined(mips) || defined(__mips__) || defined(__mips)
        return "MIPS";
    #elif defined(__sh__)
        return "SUPERH";
    #elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
        return "POWERPC";
    #elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
        return "POWERPC64";
    #elif defined(__sparc__) || defined(__sparc)
        return "SPARC";
    #elif defined(__m68k__)
        return "M68K";
    #else
        return "UNKNOWN";
    #endif
}

const char* getOS()
{
    // ======================
    // Microsoft
    // ======================
    #ifdef _WIN64
        return "Windows 64 bit";
    #elifdef _WIN32
        return "Windows 32 bit";
    #elifdef _WIN32_WCE
        return "Windows CE";
    #elifdef _WIN16
        return "Windows 16 bit";
    #elifdef MSDOS
        return "MSDOS";

    // ===============================
    // Open Source Operating Systems
    // ===============================
    #elifdef __ANDROID__
        return "Android";
    #elifdef __bsdi__
        return "BSD/OS";
    #elifdef __DragonFly__
        return "Dragonfly";
    #elifdef __FreeBSD__
        #ifdef __GLIBC__
            return "GNU/kFreeBSD"
        #else
            return "FreeBSD";
        #endif
    #elifdef __GNU__
        return "GNU";
    #elifdef __gnu_linux__
        return "GNU Linux";
    #elifdef __linux__
        return "Linux";
    #elifdef __NetBSD__
        return "NetBSD";
    #elifdef __unix__
        return "Unix";
    #elif BSD
        return "BSD (specifics unknown)";

    // ===================
    // Apple
    // ===================
    #elifdef __APPLE__
        return "Mac OS X+";
    #elifdef __MACH__
        return "Mac OS X+";
    #elifdef macintosh
        return "Mac OS <=9";
    #elifdef Macintosh
        return "Mac OS <=9";
    // ======================================
    // Other, possibly Open Source idk
    // ======================================
    #elifdef _AIX
        return "AIX";
    #elifdef AMIGA
        return "AmigaOS";
    #elifdef aegis
        return "Appollo Aegis";
    #elifdef __BEOS__
        return "BeOS";
    #elifdef __bg__
        return "Blue Gene";
    #elifdef __convex__
        return "ConvexOS";
    #elifdef __CYGWIN__
        return "Cygwin";
    #elifdef __DGUX__
        return "DG/UX";
    #elifdef __EMX__
        return "EMX";
    #elifdef __hiuxmpp
        return "HI-UX MPP";
    #elifdef __hpux
        return "HP-UX";
    #elifdef __INTEGRITY
        return "INTEGRITY";
    #elifdef __INTERIX
        return "INTERIX";
    #elifdef __Lynx__
        return "LynxOS";
    #elifdef __minix
        return "MINIX";
    #elifdef __MORPHOS__
        return "MorphOS";
    #elifdef __mpexl
        return "MPE/iX";
    #elifdef __MVS__
        return "z/OS";
    #elifdef __nucleus__
        return "Nucleus RTOS";
    #elifdef __OS2__
        return "OS/2";
    #elifdef __OS400_
        return "IBM OS 400";
    #elifdef __OS9000
        return "Microware OS-9";
    #elifdef __osf__
        return "Tru64 (OSF/1)";
    #elifdef __palmos__
        return "Palm OS";
    #elifdef __QNX__
        return "QNX";
    #elifdef __sgi
        return "IRIX";
    #elifdef __sun
        return "Solaris";
    #elifdef __SVR4
        return "SVR4";
    #elifdef __SYLLABLE__
        return "Syllable";
    #elifdef __SYMBIAN32__
        return "Symbian OS";
    #elifdef __TANDEM
        return "NonStop";
    #elifdef __ultrix__
        return "Ultrix";
    #elifdef __VMS
        return "VMS";
    #elifdef __VOS__
        return "Stratus VOS";
    #elifdef _CRAY
        return "UNICOS/mp";
    #elifdef _SCO_DS
        return "SCO OpenServer";
    #elifdef _SEQUENT_
        return "DYNIX/ptx";
    #elifdef _UNICOS
        return "UNICOS";
    #elifdef _UNIXWARE
        return "UnixWare";
    #elifdef _UWIN
        return "U/Win";
    #elifdef _WINDU_SOURCE
        return "Wind/U";
    #elifdef EPLAN9
        return "Plan 9";
    #elifdef pyr
        return "Pyramid DC/OSx";
    #elifdef sinux
        return "Reliant UNIX";
    #elifdef UTS
        return "Amdahl UTS";
    #else
        return "Unknown";
    #endif
    //TODO this does not contain all operating systems yet, complete some other time
}
