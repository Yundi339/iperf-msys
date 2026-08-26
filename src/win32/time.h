#ifndef IPERF_WIN32_TIME_H
#define IPERF_WIN32_TIME_H

#include_next <time.h>

#ifdef __cplusplus
extern "C" {
#endif

clock_t iperf_win32_clock(void);

#ifdef __cplusplus
}
#endif

/*
 * The Microsoft UCRT clock() reports elapsed wall-clock time, while ISO C
 * defines clock() as process CPU time.  Keep common iperf code on the ISO C
 * semantics so cpu_util() reports a meaningful total CPU percentage.
 */
#define clock iperf_win32_clock

#endif
