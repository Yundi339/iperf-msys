#ifndef IPERF_WIN32_FCNTL_H
#define IPERF_WIN32_FCNTL_H

#include_next <fcntl.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <errno.h>

#ifndef F_GETFL
#define F_GETFL 3
#endif
#ifndef F_SETFL
#define F_SETFL 4
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK 0x4000
#endif

static inline int
iperf_win32_fcntl(iperf_socket_t fd, int cmd, ...)
{
    va_list ap;
    int flags;
    u_long mode;

    if (cmd == F_GETFL) {
        int socket_type;
        int optlen = sizeof(socket_type);

        /*
         * The compatibility layer does not track nonblocking state because
         * timeout_connect() only asks about newly-created blocking sockets.
         * Still validate the handle so F_GETFL does not report success for a
         * closed or non-socket descriptor.
         */
        if (getsockopt(fd, SOL_SOCKET, SO_TYPE, &socket_type, &optlen) == SOCKET_ERROR)
            return -1;
        return 0;
    }
    if (cmd != F_SETFL) {
        errno = EINVAL;
        return -1;
    }

    va_start(ap, cmd);
    flags = va_arg(ap, int);
    va_end(ap);
    mode = (flags & O_NONBLOCK) ? 1UL : 0UL;
    if (ioctlsocket((SOCKET)(uintptr_t)fd, FIONBIO, &mode) == SOCKET_ERROR) {
        iperf_win32_set_errno(WSAGetLastError());
        return -1;
    }
    return 0;
}

#define fcntl iperf_win32_fcntl

#endif
