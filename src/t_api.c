/*
 * iperf, Copyright (c) 2017-2020, The Regents of the University of
 * California, through Lawrence Berkeley National Laboratory (subject
 * to receipt of any required approvals from the U.S. Dept. of
 * Energy).  All rights reserved.
 *
 * If you have questions about your rights to use or distribute this
 * software, please contact Berkeley Lab's Technology Transfer
 * Department at TTD@lbl.gov.
 *
 * NOTICE.  This software is owned by the U.S. Department of Energy.
 * As such, the U.S. Government has been granted for itself and others
 * acting on its behalf a paid-up, nonexclusive, irrevocable,
 * worldwide license in the Software to reproduce, prepare derivative
 * works, and perform publicly and display publicly.  Beginning five
 * (5) years after the date permission to assert copyright is obtained
 * from the U.S. Department of Energy, and subject to any subsequent
 * five (5) year renewals, the U.S. Government is granted for itself
 * and others acting on its behalf a paid-up, nonexclusive,
 * irrevocable, worldwide license in the Software to reproduce,
 * prepare derivative works, distribute copies to the public, perform
 * publicly and display publicly, and to permit others to do so.
 *
 * This code is distributed under a BSD style license, see the LICENSE
 * file for complete information.
 */


#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>

#include "iperf.h"
#include "iperf_api.h"
#include "iperf_util.h"

#include "version.h"

#include "units.h"

#ifdef _WIN32
#include "win32/iperf_win32.h"
#endif

int test_iperf_set_test_bind_port(struct iperf_test *test)
{
    int port;
    port = iperf_get_test_bind_port(test);
    iperf_set_test_bind_port(test, 5202);
    port = iperf_get_test_bind_port(test);
    assert(port == 5202);
    return 0;
}

int test_iperf_set_mss(struct iperf_test *test)
{
    int mss = iperf_get_test_mss(test);
    iperf_set_test_mss(test, 535);
    mss = iperf_get_test_mss(test);
    assert(mss == 535);
    return 0;
}

int test_iperf_udp_connect_port_state(struct iperf_test *test)
{
    iperf_set_test_server_port(test, 5203);
    test->udp_connect_port = 5204;
    assert(iperf_get_test_server_port(test) == 5203);
    return 0;
}

#ifdef _WIN32
int test_iperf_win32_errno_mapping(void)
{
    assert(iperf_win32_errno_from_wsa(0) == 0);
    assert(iperf_win32_errno_from_wsa(WSASYSNOTREADY) == ENETDOWN);
    assert(iperf_win32_errno_from_wsa(WSAVERNOTSUPPORTED) == EPROTONOSUPPORT);
    assert(iperf_win32_errno_from_wsa(WSAEPROCLIM) == EAGAIN);
    assert(iperf_win32_errno_from_wsa(WSAENOTSOCK) == EBADF);
    assert(iperf_win32_errno_from_wsa(WSAENOPROTOOPT) == ENOPROTOOPT);
    assert(iperf_win32_errno_from_wsa(WSAEPROTOTYPE) == EPROTOTYPE);
    assert(iperf_win32_errno_from_wsa(WSAEPFNOSUPPORT) == EAFNOSUPPORT);
    assert(iperf_win32_errno_from_wsa(WSAECONNREFUSED) == ECONNREFUSED);
    assert(iperf_win32_errno_from_wsa(WSAETIMEDOUT) == ETIMEDOUT);
    assert(iperf_win32_errno_from_wsa(WSAEWOULDBLOCK) == EWOULDBLOCK);
    return 0;
}

int test_iperf_win32_cpu_util_consistency(void)
{
    double pcpu[3];

    /*
     * A wall-clock clock() implementation would report roughly 100% here
     * even though the process is sleeping.  Total CPU must instead be the
     * same process user+system time represented by getrusage().
     */
    cpu_util(NULL);
    Sleep(250);
    cpu_util(pcpu);

    assert(pcpu[0] == pcpu[1] + pcpu[2]);
    return 0;
}

int test_iperf_win32_getrusage_errors(void)
{
    struct rusage usage;

    errno = 0;
    assert(getrusage(RUSAGE_SELF + 1, &usage) == -1);
    assert(errno == EINVAL);

    errno = 0;
    assert(getrusage(RUSAGE_SELF, NULL) == -1);
    assert(errno == EFAULT);

    errno = 0;
    assert(getrusage(RUSAGE_SELF, &usage) == 0);
    assert(usage.ru_utime.tv_sec >= 0);
    assert(usage.ru_stime.tv_sec >= 0);
    return 0;
}

int test_iperf_win32_is_closed_socket(void)
{
    iperf_socket_t s;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    assert(s != IPERF_INVALID_SOCKET);
    assert(is_closed(s) == 0);
    assert(IPERF_SOCKET_CLOSE(s) == 0);
    assert(is_closed(s) == 1);
    return 0;
}

int test_iperf_win32_socket_timeout_roundtrip(void)
{
    iperf_socket_t s;
    struct timeval set_timeout;
    struct timeval invalid_timeout;
    struct {
        struct timeval timeout;
        unsigned char extra[16];
    } option_buffer;
    const int timeout_options[] = { SO_RCVTIMEO, SO_SNDTIMEO };
    DWORD native_timeout;
    DWORD got_native_timeout;
    int optlen;
    size_t i;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    assert(s != IPERF_INVALID_SOCKET);

    set_timeout.tv_sec = 1;
    set_timeout.tv_usec = 250000;

    for (i = 0; i < sizeof(timeout_options) / sizeof(timeout_options[0]); ++i) {
        memset(&option_buffer, 0, sizeof(option_buffer));
        option_buffer.timeout = set_timeout;
        assert(setsockopt(s, SOL_SOCKET, timeout_options[i],
                          &option_buffer, sizeof(option_buffer)) == 0);

        memset(&option_buffer, 0, sizeof(option_buffer));
        optlen = sizeof(option_buffer);
        assert(getsockopt(s, SOL_SOCKET, timeout_options[i],
                          &option_buffer, &optlen) == 0);
        assert(optlen == (int)sizeof(option_buffer.timeout));
        assert(option_buffer.timeout.tv_sec == set_timeout.tv_sec);
        assert(option_buffer.timeout.tv_usec == set_timeout.tv_usec);

        /* Preserve native Winsock DWORD timeout callers as well. */
        native_timeout = 1500U;
        assert(setsockopt(s, SOL_SOCKET, timeout_options[i],
                          &native_timeout, sizeof(native_timeout)) == 0);
        got_native_timeout = 0;
        optlen = sizeof(got_native_timeout);
        assert(getsockopt(s, SOL_SOCKET, timeout_options[i],
                          &got_native_timeout, &optlen) == 0);
        assert(optlen == (int)sizeof(got_native_timeout));
        assert(got_native_timeout == native_timeout);

        invalid_timeout.tv_sec = -1;
        invalid_timeout.tv_usec = 0;
        errno = 0;
        assert(setsockopt(s, SOL_SOCKET, timeout_options[i],
                          &invalid_timeout, sizeof(invalid_timeout)) == SOCKET_ERROR);
        assert(errno == EINVAL);

        invalid_timeout.tv_sec = 0;
        invalid_timeout.tv_usec = 1000000;
        errno = 0;
        assert(setsockopt(s, SOL_SOCKET, timeout_options[i],
                          &invalid_timeout, sizeof(invalid_timeout)) == SOCKET_ERROR);
        assert(errno == EINVAL);
    }

    assert(IPERF_SOCKET_CLOSE(s) == 0);
    return 0;
}
#endif

int
main(int argc, char **argv)
{
    const char *ver;
    struct iperf_test *test;
    int sint, gint;

    ver = iperf_get_iperf_version();
    assert(strcmp(ver, IPERF_VERSION) == 0);

    test = iperf_new_test();
    assert(test != NULL);

    iperf_defaults(test);

    sint = 10;
    iperf_set_test_connect_timeout(test, sint);
    gint = iperf_get_test_connect_timeout(test);
    assert(sint == gint);

    int ret;
    ret = test_iperf_set_test_bind_port(test);

    ret += test_iperf_set_mss(test);

    ret += test_iperf_udp_connect_port_state(test);

#ifdef _WIN32
    ret += test_iperf_win32_errno_mapping();
    ret += test_iperf_win32_cpu_util_consistency();
    ret += test_iperf_win32_getrusage_errors();
    ret += test_iperf_win32_is_closed_socket();
    ret += test_iperf_win32_socket_timeout_roundtrip();
#endif

    iperf_free_test(test);

    if (ret < 0)
    {
        return -1;
    }
    return 0;
}
