iperf3-msys2:  A TCP, UDP, and SCTP network bandwidth measurement tool (Windows UCRT version)
============================================================================================

## 🌐 Multi-language Support

<div align="center">

[![English](https://img.shields.io/badge/English-README.md-blue?style=for-the-badge)](README.md)
[![中文简体](https://img.shields.io/badge/中文简体-README_cn.md-red?style=for-the-badge)](README_cn.md)

</div>

---

## Summary

iperf3-msys2 is a Windows UCRT-adapted version of iperf3, designed for embedded Windows application development. The original iperf3 has excellent stability in POSIX/Cygwin environments, but poor adaptation for MSYS2 UCRT. This version provides specific optimizations for UCRT environments.

iperf3 is a tool for active measurements of the maximum achievable bandwidth on IP networks. It supports tuning of various parameters related to timing, protocols, and buffers. For each test it reports the measured throughput / bitrate, loss, and other parameters.

This version is based on iperf3, which is a redesign of an original version developed at NLANR/DAST. iperf3 is a new implementation from scratch, with the goal of a smaller, simpler code base, and a library version of the functionality that can be used in other programs. iperf3 also has a number of features found in other tools such as nuttcp and netperf, but were missing from the original iperf. These include, for example, a zero-copy mode and optional JSON output. Note that iperf3 is *not* backwards compatible with the original iperf.

## Windows UCRT Adaptations

This version is specifically designed for Windows UCRT application development and includes the following improvements:

- **UCRT Adaptation**: Optimized specifically for Windows Universal C Runtime environment
- **Source Code Integration**: Can be integrated into Windows UCRT projects
- **UCRT Stability**: Fixed client termination crashes in UCRT environment
- **Resource Management**: Improved thread cleanup and socket handling for UCRT
- **Windows Socket Support**: Adapted for Windows socket operations and cleanup
- **New Ecosystem**: Provides new development options for MSYS2 UCRT environment

## Known Issues

⚠️ **Current Limitations:**
- UDP socket leaks may still occur in some edge cases when clients disconnect unexpectedly
- TCP connections are generally more stable than UDP in this version
- Some advanced features may require additional testing in embedded environments

## Original Project

The original iperf3 is principally developed by ESnet / Lawrence Berkeley National Laboratory. It is released under a three-clause BSD license.

For more information about the original iperf3:
- Official website: https://software.es.net/iperf
- Original source code: https://github.com/esnet/iperf
- Discussion forums: https://github.com/esnet/iperf/discussions

## Obtaining iperf3-msys2

To get the MSYS2-optimized version, clone this repository:

    git clone https://github.com/Yundi339/iperf-msys.git
    cd iperf-msys

For the original iperf3 downloads, visit:
    https://downloads.es.net/pub/iperf/

## Building iperf3-msys2

### Prerequisites

- Windows development environment with UCRT support
- MSYS2 environment with MinGW-w64
- GCC compiler (MinGW-w64)
- OpenSSL development libraries

### System Requirements

#### UCRT Runtime Requirements

<table>
    <caption id="ucrt_runtime">UCRT Runtime Requirements</caption>
    <tr>
        <th>Component</th>
        <th>Requirement</th>
    </tr>
    <tr>
        <td rowspan="3">Operating System</td>
        <td>Windows 10 version 1511 (Build 10586) or later</td>
    </tr>
    <tr>
        <td>Windows Server 2016 or later</td>
    </tr>
    <tr>
        <td>Windows IoT Core 10.0.10586 or later</td>
    </tr>
    <tr>
        <td>UCRT Version</td>
        <td>Universal C Runtime (UCRT) 10.0.10240.0 or later</td>
    </tr>
    <tr>
        <td>Architecture Support</td>
        <td>x86, x64, ARM, ARM64</td>
    </tr>
    <tr>
        <td>Network Support</td>
        <td>Network interface supporting TCP/UDP/SCTP protocols</td>
    </tr>
</table>

#### Development Environment Requirements

<table>
    <caption id="development_requirements">Development Environment Requirements</caption>
    <tr>
        <th>Component</th>
        <th>Requirement</th>
    </tr>
    <tr>
        <td>Development Environment</td>
        <td>MSYS2 + MinGW-w64 (recommended)</td>
    </tr>
    <tr>
        <td>Compiler</td>
        <td>GCC 8.0+ (MinGW-w64)</td>
    </tr>
    <tr>
        <td>Dependencies</td>
        <td>OpenSSL 1.1.1+ development libraries</td>
    </tr>
    <tr>
        <td>Build Tools</td>
        <td>autoconf, automake, libtool</td>
    </tr>
</table>

> [!NOTE]
> This project is specifically designed for UCRT environments using MinGW-w64 compiler. MSVC uses MSVCRT, which is incompatible with this project's UCRT adaptation.

### Building with MSYS2

```bash
# Install dependencies
pacman -S base-devel openssl-devel

# Generate configure script
./bootstrap.sh

# Configure build - static compilation
./configure \
  --prefix=/ucrt64 \
  --host=x86_64-w64-mingw32 \
  --build=x86_64-w64-mingw32 \
  --disable-shared \
  --enable-static \
  CFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions --param=ssp-buffer-size=4" \
  CXXFLAGS="-O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions --param=ssp-buffer-size=4" \
  LDFLAGS="-static -Wl,--enable-auto-import -Wl,--enable-runtime-pseudo-reloc" \
  LIBS="-lssl -lcrypto -lpthread -lws2_32 -lcrypt32" \
  PKG_CONFIG_PATH="/ucrt64/lib/pkgconfig"

# Compile
make -j$(nproc)

# Run tests
make check

# Install
make install
```


### Building for UCRT Projects

This version is specifically designed for integration into Windows UCRT projects. The UCRT compatibility makes it suitable for:

- Windows IoT applications
- Embedded Windows systems
- UCRT-based Windows applications
- Direct source code integration into your projects

#### Integration into UCRT Projects

```bash
# Copy source files to your UCRT project
cp -r src/ your-ucrt-project/iperf3/

# Include necessary source files in your project
# Main files: iperf_api.c, iperf_server_api.c, iperf_client_api.c
# Network files: net.c, iperf_tcp.c, iperf_udp.c
# Utility files: iperf_util.c, iperf_error.c, iperf_time.c
```

## Project Features

### 🎯 Windows UCRT Adaptation
- **UCRT Optimization**: Specifically optimized for Windows Universal C Runtime environment
- **New Ecosystem**: Provides new development options for MSYS2 UCRT environment
- **Environment Adaptation**: Solves adaptation issues of the original version in UCRT environment
- **Stability**: Fixed multiple stability issues in UCRT environment

### 🔧 Technical Features
- Specifically optimized for MSYS2 UCRT environment
- Provides Windows UCRT native network performance testing capabilities
- Supports Windows IoT, embedded systems and other UCRT environments
- Can be compiled into Windows UCRT projects

## Using iperf3-msys2

iperf3-msys2 includes a manual page listing all of the command-line options. The manual page is the most up-to-date reference to the various flags and parameters.

For sample command line usage, see:

https://fasterdata.es.net/performance-testing/network-troubleshooting-tools/iperf/

Using the default options, iperf is meant to show typical well
designed application performance.  "Typical well designed application"
means avoiding artificial enhancements that work only for testing
(such as splice()'ing the data to /dev/null).  iperf does also have
flags for "extreme best case" optimizations, but they must be
explicitly activated.

These flags include:

    -Z, --zerocopy            use a 'zero copy' sendfile() method of sending data
    -A, --affinity n/n,m      set CPU affinity

## Bug Reports and Issues

For issues specific to the MSYS2 version (iperf3-msys2), please submit to this repository:

https://github.com/Yundi339/iperf-msys/issues

For issues with the original iperf3, please submit to the original repository:

https://github.com/esnet/iperf/issues

### Before Submitting a Bug Report

Please make sure you're running the latest version of the code, and confirm that your issue has not already been fixed. In your issue submission, please indicate:

- The version of iperf3-msys2 you're running
- Your platform information (MSYS2 version, Windows version, etc.)
- Exact command-line arguments used
- Error messages (verbatim if possible, but sanitize any sensitive information)

### Security Issues

If you suspect there is a potential security issue with the MSYS2 version, please contact the maintainer through GitHub issues or the original iperf3 security contact at iperf@es.net.

## Relation to iperf 2.x

Although iperf2 and iperf3 both measure network performance, they are not compatible with each other. The projects (as of mid-2021) are in active, but separate, development. The continuing iperf2 development project can be found at https://sourceforge.net/projects/iperf2/.

## Development Status

### ✅ Fixed Issues

This UCRT version has addressed several issues present in the original iperf3:

- ✅ Fixed client termination crashes
- ✅ Improved select() error handling  
- ✅ Better thread cleanup and resource management
- ✅ Enhanced error recovery for network disconnections
- ✅ UCRT compatibility for Windows embedded development

### ⚠️ Known Limitations

- **UDP Socket Leaks**: In some edge cases, UDP socket leaks may still occur when clients disconnect unexpectedly, potentially leading to core dumps
- **TCP vs UDP Stability**: TCP connections are generally more stable than UDP in this version
- **Embedded Testing**: Some advanced features may require additional testing in embedded environments
- **Resource Monitoring**: Consider implementing additional resource monitoring in production embedded applications

### Original iperf3 Issues

For known issues with the original iperf3, see:
https://software.es.net/iperf/dev.html#known-issues

## Useful Links

### MSYS2 Resources
- [MSYS2 Official Website](https://www.msys2.org/)
- [MSYS2 Installation Guide](https://www.msys2.org/wiki/Installation/)

### iperf3 Resources
This section lists links to user-contributed Web pages regarding iperf3. ESnet and Lawrence Berkeley National Laboratory bear no responsibility for the content of these pages.

* Installation instructions for Debian Linux (by Cameron Camp <cameron@ivdatacenter.com>):
  http://cheatsheet.logicalwebhost.com/iperf-network-testing/

## Copyright and License

### Original iperf3

This UCRT version (iperf3-msys2) is based on iperf3, which is:

Copyright (c) 2014-2025, The Regents of the University of California, through Lawrence Berkeley National Laboratory (subject to receipt of any required approvals from the U.S. Dept. of Energy). All rights reserved.

The original iperf3 is distributed under a three-clause BSD license. See the LICENSE file for complete information.

### UCRT Adaptations

The UCRT-specific adaptations and improvements in this version are provided under the same BSD license as the original iperf3.

### Important Notes for Developers

- This is a community-driven adaptation for Windows UCRT environments
- The original iperf3 copyright and license terms apply to the base code
- UCRT-specific modifications are made available under the same BSD license
- This version is intended for embedded Windows application development
- Please ensure compliance with the original iperf3 license terms when using this code

### Contact

For questions about the original iperf3 license, contact Berkeley Lab's Technology Transfer Department at TTD@lbl.gov.

For questions about this UCRT adaptation, please use the GitHub issues at [https://github.com/Yundi339/iperf-msys](https://github.com/Yundi339/iperf-msys).

### 🌟 Star History

[![Star History Chart](https://api.star-history.com/svg?repos=Yundi339/iperf-msys&type=Date)](https://www.star-history.com/#Yundi339/iperf-msys&Date)

---

**iperf3-msys2 - Windows UCRT Network Performance Testing Tool**

<div align="center">

**Maintainer**: [Yundi339](https://github.com/Yundi339) | **Repository**: [https://github.com/Yundi339/iperf-msys](https://github.com/Yundi339/iperf-msys)

</div>
