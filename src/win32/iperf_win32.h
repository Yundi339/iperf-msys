#ifndef IPERF_WIN32_H
#define IPERF_WIN32_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <pthread.h>
#include <errno.h>

int iperf_win32_errno_from_wsa(int error);
void iperf_win32_set_errno(int error);
int iperf_win32_errno_from_system(DWORD error);
void iperf_win32_set_system_errno(DWORD error);
BOOL iperf_win32_get_process_affinity_mask(HANDLE process, PDWORD_PTR process_mask, PDWORD_PTR system_mask);
BOOL iperf_win32_set_process_affinity_mask(HANDLE process, DWORD_PTR process_mask);
int iperf_win32_init(void);
int iperf_win32_process_exists(long pid);
int iperf_win32_kill(long pid, int sig);
int iperf_win32_cancel_thread_io(pthread_t thread);
int iperf_win32_wait_readable(SOCKET socket, unsigned int timeout_ms);
const char *iperf_win32_signal_name(int sig);

/*
 * These two Win32 APIs are used by the common CPU-affinity code.  Keep their
 * return values unchanged while making failures obey the errno contract used
 * by the rest of iperf.
 */
#define GetProcessAffinityMask(process, process_mask, system_mask) \
    iperf_win32_get_process_affinity_mask((process), (process_mask), (system_mask))
#define SetProcessAffinityMask(process, process_mask) \
    iperf_win32_set_process_affinity_mask((process), (process_mask))

#define kill(pid, sig) iperf_win32_kill((pid), (sig))
#define strsignal(sig) iperf_win32_signal_name((sig))

#endif
