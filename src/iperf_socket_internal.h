#ifndef IPERF_SOCKET_INTERNAL_H
#define IPERF_SOCKET_INTERNAL_H

#include "iperf_socket.h"
#include <unistd.h>

#ifdef _WIN32
#define IPERF_SOCKET_CLOSE(value) iperf_win32_close_socket((value))
#else
#define IPERF_SOCKET_CLOSE(value) close((value))
#endif

#endif
