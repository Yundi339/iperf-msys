#ifndef IPERF_WIN32_POLL_H
#define IPERF_WIN32_POLL_H

#include <sys/socket.h>

#ifndef POLLOUT
#define POLLOUT POLLWRNORM
#endif

static inline int
iperf_win32_poll(struct pollfd *fds, unsigned long nfds, int timeout)
{
    int rc = WSAPoll((LPWSAPOLLFD)fds, (ULONG)nfds, timeout);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

#define poll(fds, nfds, timeout) iperf_win32_poll((fds), (nfds), (timeout))

#endif
