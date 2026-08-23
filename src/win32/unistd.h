#ifndef IPERF_WIN32_UNISTD_H
#define IPERF_WIN32_UNISTD_H

#include_next <unistd.h>
#include <sys/socket.h>
#include <stdint.h>

static inline int
iperf_win32_close_socket(iperf_socket_t fd)
{
    SOCKET sock = (SOCKET)(uintptr_t)fd;

    if (closesocket(sock) == 0)
        return 0;

    iperf_win32_set_errno(WSAGetLastError());
    return -1;
}

#endif
