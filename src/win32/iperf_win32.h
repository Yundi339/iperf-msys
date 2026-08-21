#ifndef IPERF_WIN32_H
#define IPERF_WIN32_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <pthread.h>
#include <errno.h>

int iperf_win32_init(void);
int iperf_win32_process_exists(long pid);
int iperf_win32_kill(long pid, int sig);
int iperf_win32_cancel_thread_io(pthread_t thread);
int iperf_win32_wait_readable(SOCKET socket, unsigned int timeout_ms);
const char *iperf_win32_signal_name(int sig);

#define kill(pid, sig) iperf_win32_kill((pid), (sig))
#define strsignal(sig) iperf_win32_signal_name((sig))

#endif
