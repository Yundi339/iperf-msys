#ifndef IPERF_SOCKET_H
#define IPERF_SOCKET_H

#include <stdint.h>
#include <inttypes.h>

#ifdef _WIN32
typedef intptr_t iperf_socket_t;
#else
typedef int iperf_socket_t;
#endif

#define IPERF_INVALID_SOCKET ((iperf_socket_t)-1)

#ifdef _WIN32
#define IPERF_SOCKET_FORMAT PRIdPTR
#define IPERF_SOCKET_FORMAT_ARG(value) ((intptr_t)(value))
#else
#define IPERF_SOCKET_FORMAT "d"
#define IPERF_SOCKET_FORMAT_ARG(value) (value)
#endif

#endif
