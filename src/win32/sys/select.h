#ifndef IPERF_WIN32_SYS_SELECT_H
#define IPERF_WIN32_SYS_SELECT_H

#include <sys/socket.h>

static inline int
iperf_win32_select(fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
    const struct timeval *timeout)
{
    int rc = select(0, readfds, writefds, exceptfds, timeout);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

#define select(nfds, readfds, writefds, exceptfds, timeout) \
    iperf_win32_select((readfds), (writefds), (exceptfds), (timeout))

#endif
