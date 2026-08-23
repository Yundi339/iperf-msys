#ifndef IPERF_WIN32_SYS_UTSNAME_H
#define IPERF_WIN32_SYS_UTSNAME_H

#include <windows.h>
#include <stdio.h>
#include <string.h>

struct utsname {
    char sysname[64];
    char nodename[256];
    char release[64];
    char version[64];
    char machine[64];
};

static inline int
uname(struct utsname *name)
{
    DWORD n = (DWORD)sizeof(name->nodename);
    SYSTEM_INFO info;

    memset(name, 0, sizeof(*name));
    snprintf(name->sysname, sizeof(name->sysname), "Windows");
    if (!GetComputerNameA(name->nodename, &n))
        snprintf(name->nodename, sizeof(name->nodename), "unknown");
    snprintf(name->release, sizeof(name->release), "UCRT");
    snprintf(name->version, sizeof(name->version), "Win32");

    GetNativeSystemInfo(&info);
    switch (info.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64:
        snprintf(name->machine, sizeof(name->machine), "x86_64");
        break;
    case PROCESSOR_ARCHITECTURE_ARM64:
        snprintf(name->machine, sizeof(name->machine), "arm64");
        break;
    case PROCESSOR_ARCHITECTURE_INTEL:
        snprintf(name->machine, sizeof(name->machine), "x86");
        break;
    default:
        snprintf(name->machine, sizeof(name->machine), "unknown");
        break;
    }
    return 0;
}

#endif
