#ifndef IPERF_WIN32_SYS_MMAN_H
#define IPERF_WIN32_SYS_MMAN_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#include <stdint.h>
#include <sys/types.h>

#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define MAP_SHARED 0x01
#define MAP_PRIVATE 0x02
#define MAP_FAILED ((void *)-1)

static inline void *
mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    HANDLE file_handle;
    HANDLE mapping;
    ULONGLONG max_size;
    DWORD access;
    void *view;
    (void)addr;
    (void)prot;

    file_handle = (HANDLE)_get_osfhandle(fd);
    if (file_handle == INVALID_HANDLE_VALUE)
        return MAP_FAILED;

    max_size = (ULONGLONG)offset + (ULONGLONG)length;
    mapping = CreateFileMappingA(file_handle, NULL, PAGE_READWRITE,
                                 (DWORD)(max_size >> 32), (DWORD)max_size, NULL);
    if (mapping == NULL)
        return MAP_FAILED;

    access = (flags & MAP_PRIVATE) ? FILE_MAP_COPY : FILE_MAP_ALL_ACCESS;
    view = MapViewOfFile(mapping, access,
                         (DWORD)(((ULONGLONG)offset) >> 32), (DWORD)offset, length);
    CloseHandle(mapping);
    return view == NULL ? MAP_FAILED : view;
}

static inline int
munmap(void *addr, size_t length)
{
    (void)length;
    return UnmapViewOfFile(addr) ? 0 : -1;
}

#endif
