#ifndef IPERF_WIN32_SYS_SOCKET_H
#define IPERF_WIN32_SYS_SOCKET_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>
#include "../../iperf_socket.h"

#ifndef SHUT_RD
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH
#endif

static inline void
iperf_win32_set_errno(int error)
{
    switch (error) {
    case WSAEWOULDBLOCK:
        errno = EWOULDBLOCK;
        break;
    case WSAEINPROGRESS:
        errno = EINPROGRESS;
        break;
#ifdef WSAEALREADY
    case WSAEALREADY:
        errno = EALREADY;
        break;
#endif
    case WSAEACCES:
        errno = EACCES;
        break;
    case WSAEFAULT:
        errno = EFAULT;
        break;
    case WSAEINVAL:
        errno = EINVAL;
        break;
    case WSAEBADF:
    case WSAENOTSOCK:
        errno = EBADF;
        break;
    case WSAEMFILE:
        errno = EMFILE;
        break;
    case WSAETIMEDOUT:
        errno = ETIMEDOUT;
        break;
    case WSAECONNREFUSED:
        errno = ECONNREFUSED;
        break;
    case WSAECONNRESET:
        errno = ECONNRESET;
        break;
    case WSAECONNABORTED:
        errno = ECONNABORTED;
        break;
    case WSAENOTCONN:
        errno = ENOTCONN;
        break;
    case WSAEISCONN:
        errno = EISCONN;
        break;
    case WSAESHUTDOWN:
        errno = EPIPE;
        break;
    case WSAEADDRINUSE:
        errno = EADDRINUSE;
        break;
    case WSAEADDRNOTAVAIL:
        errno = EADDRNOTAVAIL;
        break;
    case WSAEAFNOSUPPORT:
        errno = EAFNOSUPPORT;
        break;
    case WSAEDESTADDRREQ:
        errno = EDESTADDRREQ;
        break;
    case WSAENETDOWN:
        errno = ENETDOWN;
        break;
    case WSAENETRESET:
        errno = ENETRESET;
        break;
    case WSAENETUNREACH:
        errno = ENETUNREACH;
        break;
    case WSAEHOSTUNREACH:
        errno = EHOSTUNREACH;
        break;
    case WSAENOBUFS:
        errno = ENOBUFS;
        break;
    case WSAEMSGSIZE:
        errno = EMSGSIZE;
        break;
    case WSAEPROTONOSUPPORT:
        errno = EPROTONOSUPPORT;
        break;
    case WSAEOPNOTSUPP:
        errno = EOPNOTSUPP;
        break;
    case WSAEINTR:
#ifdef WSA_OPERATION_ABORTED
    case WSA_OPERATION_ABORTED:
#endif
        errno = EINTR;
        break;
    default:
        errno = EIO;
        break;
    }
}

static inline iperf_socket_t
iperf_win32_adopt_socket(SOCKET s)
{
    if (s == INVALID_SOCKET) {
        iperf_win32_set_errno(WSAGetLastError());
        return IPERF_INVALID_SOCKET;
    }
    if ((uintptr_t)s > (uintptr_t)INTPTR_MAX) {
        closesocket(s);
        errno = EOVERFLOW;
        return IPERF_INVALID_SOCKET;
    }
    return (iperf_socket_t)(intptr_t)(uintptr_t)s;
}

static inline iperf_socket_t
iperf_win32_socket(int af, int type, int protocol)
{
    return iperf_win32_adopt_socket(socket(af, type, protocol));
}

static inline iperf_socket_t
iperf_win32_accept(iperf_socket_t s, struct sockaddr *addr, int *addrlen)
{
    return iperf_win32_adopt_socket(accept((SOCKET)(uintptr_t)s, addr, addrlen));
}

static inline int
iperf_win32_connect(iperf_socket_t s, const struct sockaddr *name, int namelen)
{
    int rc = connect((SOCKET)(uintptr_t)s, name, namelen);
    if (rc == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error == WSAEWOULDBLOCK || error == WSAEINPROGRESS)
            errno = EINPROGRESS;
#ifdef WSAEALREADY
        else if (error == WSAEALREADY)
            errno = EALREADY;
#endif
        else
            iperf_win32_set_errno(error);
    }
    return rc;
}

static inline int
iperf_win32_bind(iperf_socket_t s, const struct sockaddr *name, int namelen)
{
    int rc = bind((SOCKET)(uintptr_t)s, name, namelen);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_listen(iperf_socket_t s, int backlog)
{
    int rc = listen((SOCKET)(uintptr_t)s, backlog);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_shutdown(iperf_socket_t s, int how)
{
    int rc = shutdown((SOCKET)(uintptr_t)s, how);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_getsockname(iperf_socket_t s, struct sockaddr *name, int *namelen)
{
    int rc = getsockname((SOCKET)(uintptr_t)s, name, namelen);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_getpeername(iperf_socket_t s, struct sockaddr *name, int *namelen)
{
    int rc = getpeername((SOCKET)(uintptr_t)s, name, namelen);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_setsockopt(iperf_socket_t s, int level, int optname, const void *optval, int optlen)
{
    int rc = setsockopt((SOCKET)(uintptr_t)s, level, optname, (const char *) optval, optlen);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_getsockopt(iperf_socket_t s, int level, int optname, void *optval, int *optlen)
{
    int rc = getsockopt((SOCKET)(uintptr_t)s, level, optname, (char *) optval, optlen);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_recv(iperf_socket_t s, void *buf, int len, int flags)
{
    int rc = recv((SOCKET)(uintptr_t)s, (char *) buf, len, flags);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_recvfrom(iperf_socket_t s, void *buf, int len, int flags,
    struct sockaddr *from, int *fromlen)
{
    int rc = recvfrom((SOCKET)(uintptr_t)s, (char *) buf, len, flags, from, fromlen);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_send(iperf_socket_t s, const void *buf, int len, int flags)
{
    int rc = send((SOCKET)(uintptr_t)s, (const char *) buf, len, flags);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

static inline int
iperf_win32_sendto(iperf_socket_t s, const void *buf, int len, int flags,
    const struct sockaddr *to, int tolen)
{
    int rc = sendto((SOCKET)(uintptr_t)s, (const char *) buf, len, flags, to, tolen);
    if (rc == SOCKET_ERROR)
        iperf_win32_set_errno(WSAGetLastError());
    return rc;
}

#define socket(af, type, protocol) iperf_win32_socket((af), (type), (protocol))
#define accept(s, addr, addrlen) iperf_win32_accept((s), (addr), (addrlen))
#define connect(s, name, namelen) iperf_win32_connect((s), (name), (namelen))
#define bind(s, name, namelen) iperf_win32_bind((s), (name), (namelen))
#define listen(s, backlog) iperf_win32_listen((s), (backlog))
#define shutdown(s, how) iperf_win32_shutdown((s), (how))
#define getsockname(s, name, namelen) iperf_win32_getsockname((s), (name), (namelen))
#define getpeername(s, name, namelen) iperf_win32_getpeername((s), (name), (namelen))
#define setsockopt(s, level, optname, optval, optlen) \
    iperf_win32_setsockopt((s), (level), (optname), (optval), (optlen))
#define getsockopt(s, level, optname, optval, optlen) \
    iperf_win32_getsockopt((s), (level), (optname), (optval), (optlen))
#define recv(s, buf, len, flags) iperf_win32_recv((s), (buf), (len), (flags))
#define recvfrom(s, buf, len, flags, from, fromlen) \
    iperf_win32_recvfrom((s), (buf), (len), (flags), (from), (fromlen))
#define send(s, buf, len, flags) iperf_win32_send((s), (buf), (len), (flags))
#define sendto(s, buf, len, flags, to, tolen) \
    iperf_win32_sendto((s), (buf), (len), (flags), (to), (tolen))

#endif
