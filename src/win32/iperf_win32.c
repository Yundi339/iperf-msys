#include "iperf_win32.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>

static INIT_ONCE iperf_wsa_once = INIT_ONCE_STATIC_INIT;
static int iperf_wsa_result = WSASYSNOTREADY;

int
iperf_win32_errno_from_wsa(int error)
{
    switch (error) {
    case 0:
        return 0;
    case WSASYSNOTREADY:
        return ENETDOWN;
    case WSAVERNOTSUPPORTED:
        return EPROTONOSUPPORT;
    case WSAEPROCLIM:
        return EAGAIN;
    case WSAEWOULDBLOCK:
        return EWOULDBLOCK;
    case WSAEINPROGRESS:
        return EINPROGRESS;
#ifdef WSAEALREADY
    case WSAEALREADY:
        return EALREADY;
#endif
    case WSAEACCES:
        return EACCES;
    case WSAEFAULT:
        return EFAULT;
    case WSAEINVAL:
        return EINVAL;
    case WSAEBADF:
    case WSAENOTSOCK:
        return EBADF;
    case WSAEMFILE:
        return EMFILE;
    case WSAETIMEDOUT:
        return ETIMEDOUT;
    case WSAECONNREFUSED:
        return ECONNREFUSED;
    case WSAECONNRESET:
        return ECONNRESET;
    case WSAECONNABORTED:
        return ECONNABORTED;
    case WSAENOTCONN:
        return ENOTCONN;
    case WSAEISCONN:
        return EISCONN;
    case WSAESHUTDOWN:
        return EPIPE;
    case WSAEADDRINUSE:
        return EADDRINUSE;
    case WSAEADDRNOTAVAIL:
        return EADDRNOTAVAIL;
    case WSAEAFNOSUPPORT:
    case WSAEPFNOSUPPORT:
        return EAFNOSUPPORT;
    case WSAEDESTADDRREQ:
        return EDESTADDRREQ;
    case WSAENETDOWN:
        return ENETDOWN;
    case WSAENETRESET:
        return ENETRESET;
    case WSAENETUNREACH:
        return ENETUNREACH;
    case WSAEHOSTDOWN:
    case WSAEHOSTUNREACH:
        return EHOSTUNREACH;
    case WSAENOBUFS:
        return ENOBUFS;
    case WSAEMSGSIZE:
        return EMSGSIZE;
    case WSAEPROTOTYPE:
        return EPROTOTYPE;
    case WSAENOPROTOOPT:
        return ENOPROTOOPT;
    case WSAEPROTONOSUPPORT:
    case WSAESOCKTNOSUPPORT:
        return EPROTONOSUPPORT;
    case WSAENAMETOOLONG:
        return ENAMETOOLONG;
    case WSAELOOP:
        return ELOOP;
    case WSAEOPNOTSUPP:
        return EOPNOTSUPP;
    case WSAEINTR:
#ifdef WSA_OPERATION_ABORTED
    case WSA_OPERATION_ABORTED:
#endif
        return EINTR;
    default:
        return EIO;
    }
}

void
iperf_win32_set_errno(int error)
{
    errno = iperf_win32_errno_from_wsa(error);
}

static BOOL CALLBACK
iperf_win32_start_winsock(PINIT_ONCE once, PVOID parameter, PVOID *context)
{
    WSADATA data;
    (void)once;
    (void)parameter;
    (void)context;
    iperf_wsa_result = WSAStartup(MAKEWORD(2, 2), &data);
    return TRUE;
}

int
iperf_win32_init(void)
{
    if (!InitOnceExecuteOnce(&iperf_wsa_once, iperf_win32_start_winsock, NULL, NULL)) {
        errno = EIO;
        return -1;
    }
    if (iperf_wsa_result != 0) {
        iperf_win32_set_errno(iperf_wsa_result);
        return -1;
    }
    return 0;
}

int
iperf_win32_process_exists(long pid)
{
    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, (DWORD)pid);
    if (process == NULL) {
        DWORD error = GetLastError();
        if (error == ERROR_ACCESS_DENIED) {
            errno = EPERM;
            return 1;
        }
        if (error == ERROR_INVALID_PARAMETER) {
            errno = ESRCH;
            return 0;
        }
        errno = EIO;
        return -1;
    }
    CloseHandle(process);
    return 1;
}

int
iperf_win32_kill(long pid, int sig)
{
    int exists;

    if (sig != 0) {
        errno = ENOSYS;
        return -1;
    }

    errno = 0;
    exists = iperf_win32_process_exists(pid);
    if (exists > 0)
        return errno == EPERM ? -1 : 0;
    return -1;
}

int
iperf_win32_cancel_thread_io(pthread_t thread)
{
    HANDLE handle = (HANDLE)pthread_gethandle(thread);
    DWORD error;

    if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
        errno = ESRCH;
        return -1;
    }

    if (CancelSynchronousIo(handle))
        return 0;

    error = GetLastError();
    if (error == ERROR_NOT_FOUND)
        return 0;
    if (error == ERROR_ACCESS_DENIED)
        errno = EPERM;
    else
        errno = EIO;
    return -1;
}

int
iperf_win32_wait_readable(SOCKET socket, unsigned int timeout_ms)
{
    fd_set read_set;
    struct timeval timeout;
    int rc;

    FD_ZERO(&read_set);
    FD_SET(socket, &read_set);
    timeout.tv_sec = (long)(timeout_ms / 1000U);
    timeout.tv_usec = (long)((timeout_ms % 1000U) * 1000U);

    rc = select(0, &read_set, NULL, NULL, &timeout);
    if (rc == SOCKET_ERROR) {
        iperf_win32_set_errno(WSAGetLastError());
        return -1;
    }
    return rc;
}

const char *
iperf_win32_signal_name(int sig)
{
    static _Thread_local char buffer[32];

    switch (sig) {
    case SIGINT:
        return "SIGINT";
    case SIGTERM:
        return "SIGTERM";
    case SIGABRT:
        return "SIGABRT";
    default:
        snprintf(buffer, sizeof(buffer), "Signal %d", sig);
        return buffer;
    }
}
