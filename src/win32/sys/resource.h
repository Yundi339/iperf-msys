#ifndef IPERF_WIN32_SYS_RESOURCE_H
#define IPERF_WIN32_SYS_RESOURCE_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

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

    if (who != RUSAGE_SELF) {
        errno = EINVAL;
        return -1;
    }
    if (usage == NULL) {
        errno = EFAULT;
        return -1;
    }

    memset(usage, 0, sizeof(*usage));
    if (!GetProcessTimes(GetCurrentProcess(), &creation, &exit_time, &kernel, &user)) {
        errno = EIO;
        return -1;
    }
    iperf_filetime_to_timeval(&user, &usage->ru_utime);
    iperf_filetime_to_timeval(&kernel, &usage->ru_stime);
    return 0;
}

#endif
