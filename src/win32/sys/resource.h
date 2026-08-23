#ifndef IPERF_WIN32_SYS_RESOURCE_H
#define IPERF_WIN32_SYS_RESOURCE_H

#include <windows.h>
#include <sys/time.h>
#include <string.h>

#define RUSAGE_SELF 0

struct rusage {
    struct timeval ru_utime;
    struct timeval ru_stime;
};

static inline void
iperf_filetime_to_timeval(const FILETIME *ft, struct timeval *tv)
{
    ULARGE_INTEGER value;
    value.LowPart = ft->dwLowDateTime;
    value.HighPart = ft->dwHighDateTime;
    tv->tv_sec = (long)(value.QuadPart / 10000000ULL);
    tv->tv_usec = (long)((value.QuadPart % 10000000ULL) / 10ULL);
}

static inline int
getrusage(int who, struct rusage *usage)
{
    FILETIME creation, exit_time, kernel, user;
    (void)who;
    memset(usage, 0, sizeof(*usage));
    if (!GetProcessTimes(GetCurrentProcess(), &creation, &exit_time, &kernel, &user))
        return -1;
    iperf_filetime_to_timeval(&user, &usage->ru_utime);
    iperf_filetime_to_timeval(&kernel, &usage->ru_stime);
    return 0;
}

#endif
