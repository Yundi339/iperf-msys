#ifndef IPERF_SOCKET_H
#define IPERF_SOCKET_H

#include <stdint.h>
#include <inttypes.h>

/*
 * iperf_api.h is a public installed header and cannot rely on the private,
 * generated iperf_config.h being present for consumers.  Recover the C11
 * atomic feature bit from the compiler when configure did not define it.
 */
#if !defined(HAVE_STDATOMIC_H) && defined(__STDC_VERSION__) && \
    __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
#define HAVE_STDATOMIC_H 1
#endif

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
