# iperf3-msys2: Windows UCRT 网络带宽测量工具

## 🌐 多语言支持 / Multi-language Support

<div align="center">

[![English](https://img.shields.io/badge/English-README.md-blue?style=for-the-badge)](README.md)
[![中文简体](https://img.shields.io/badge/中文简体-README_cn.md-red?style=for-the-badge)](README_cn.md)

</div>

---

基于ESnet/iperf3的分支，为Windows UCRT环境提供网络带宽测量功能。

**iperf3-msys2** 是一个Windows UCRT环境下的网络带宽测量工具。原始iperf3在POSIX/Cygwin环境下稳定性很好，但对MSYS2 UCRT适配较差，本版本针对UCRT环境进行了调整优化。

### 🌟 核心特性
- **UCRT适配** - 针对Windows Universal C Runtime环境进行优化
- **源码集成** - 可集成到Windows UCRT项目中
- **UCRT稳定性** - 修复UCRT环境下的客户端终止崩溃问题
- **资源管理** - 改进UCRT环境下的线程清理和socket处理
- **Windows Socket支持** - 适配Windows socket操作和清理处理
- **新生态支持** - 为MSYS2 UCRT环境提供新的开发选择

### 🖥️ Windows UCRT集成特点
- 无需外部依赖，可编译到项目中
- 支持Windows IoT、嵌入式系统等UCRT环境
- 针对MSYS2 UCRT环境进行了专门优化
- 提供Windows UCRT原生网络性能测试功能

## 系统要求

### UCRT运行时要求

<table>
    <caption id="ucrt_runtime">UCRT运行时要求</caption>
    <tr>
        <th>组件</th>
        <th>要求</th>
    </tr>
    <tr>
        <td rowspan="3">操作系统</td>
        <td>Windows 10 版本 1511 (Build 10586) 或更高版本</td>
    </tr>
    <tr>
        <td>Windows Server 2016 或更高版本</td>
    </tr>
    <tr>
        <td>Windows IoT Core 10.0.10586 或更高版本</td>
    </tr>
    <tr>
        <td>UCRT版本</td>
        <td>Universal C Runtime (UCRT) 10.0.10240.0 或更高版本</td>
    </tr>
    <tr>
        <td>架构支持</td>
        <td>x86, x64, ARM, ARM64</td>
    </tr>
    <tr>
        <td>网络支持</td>
        <td>支持TCP/UDP/SCTP协议的网络接口</td>
    </tr>
</table>

### 开发环境要求

<table>
    <caption id="development_requirements">开发环境要求</caption>
    <tr>
        <th>组件</th>
        <th>要求</th>
    </tr>
    <tr>
        <td>开发环境</td>
        <td>MSYS2 + MinGW-w64 (推荐)</td>
    </tr>
    <tr>
        <td>编译器</td>
        <td>GCC 8.0+ (MinGW-w64)</td>
    </tr>
    <tr>
        <td>依赖库</td>
        <td>OpenSSL 1.1.1+ 开发库</td>
    </tr>
    <tr>
        <td>构建工具</td>
        <td>autoconf, automake, libtool</td>
    </tr>
</table>

> [!NOTE]
> 本项目专为UCRT环境设计，使用MinGW-w64编译器。MSVC使用MSVCRT，与本项目的UCRT适配不兼容。

## 快速开始

### 📥 获取源码

```bash
git clone https://github.com/Yundi339/iperf-msys.git
cd iperf-msys
```

### 🛠️ 构建安装

#### 独立构建

```bash
# 安装依赖
pacman -S base-devel openssl-devel

# 生成configure脚本
./bootstrap.sh

# 配置构建 - 静态编译
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

# 编译
make -j$(nproc)

# 运行测试
make check

# 安装
make install
```

#### 集成到UCRT项目

```bash
# 将源码复制到您的UCRT项目中
cp -r src/ your-ucrt-project/iperf3/

# 在您的项目中包含必要的源文件
# 主要文件: iperf_api.c, iperf_server_api.c, iperf_client_api.c
# 网络文件: net.c, iperf_tcp.c, iperf_udp.c
# 工具文件: iperf_util.c, iperf_error.c, iperf_time.c
```

### 🚀 基本使用

```bash
# 启动服务器
iperf3 -s

# 客户端连接测试
iperf3 -c <服务器IP> -t 30

# UDP测试
iperf3 -c <服务器IP> -u -b 100M

# 双向测试
iperf3 -c <服务器IP> --bidir
```

## 开发状态

### ✅ 已修复问题

此UCRT版本已解决原始iperf3中的多个问题：

- ✅ 修复客户端终止崩溃问题
- ✅ 改进select()错误处理
- ✅ 更好的线程清理和资源管理
- ✅ 增强网络断连的错误恢复
- ✅ Windows UCRT兼容性支持

### ⚠️ 已知限制

- **UDP Socket泄露**: 在某些边缘情况下，客户端意外断开时仍可能发生UDP socket泄露，可能导致程序崩溃
- **TCP vs UDP稳定性**: 在此版本中，TCP连接通常比UDP更稳定
- **嵌入式测试**: 某些高级功能在嵌入式环境中可能需要额外测试
- **资源监控**: 建议在生产嵌入式应用中实施额外的资源监控

## 技术支持

遇到问题时的解决路径：
1. 查看 [原始iperf3文档](https://software.esnet/iperf/)
2. 检查 [已知问题列表](https://software.esnet/iperf/dev.html#known-issues)
3. 在GitHub上提交 [问题报告](https://github.com/Yundi339/iperf-msys/issues)
4. 查看详细的错误日志和调试信息

**问题反馈标签：**
- `ucrt-compatibility` - UCRT兼容性问题
- `socket-leak` - Socket泄露问题
- `embedded-development` - 嵌入式开发问题
- `build-issues` - 构建相关问题

## 项目特点

### 🎯 Windows UCRT适配
- **UCRT优化**: 针对Windows Universal C Runtime环境进行专门优化
- **新生态支持**: 为MSYS2 UCRT环境提供新的开发选择
- **环境适配**: 解决原始版本在UCRT环境下的适配问题
- **稳定性**: 修复UCRT环境下的多个稳定性问题

### 🔧 技术特点
- 针对MSYS2 UCRT环境进行了专门优化
- 提供Windows UCRT原生网络性能测试功能
- 支持Windows IoT、嵌入式系统等UCRT环境
- 可编译到Windows UCRT项目中

## 版权和许可

### 原始iperf3

此UCRT版本基于iperf3，原始版权：

Copyright (c) 2014-2025, The Regents of the University of California, through Lawrence Berkeley National Laboratory.

原始iperf3采用三条款BSD许可证分发。

### UCRT适配

此版本中的UCRT特定适配和改进在相同的BSD许可证下提供。

### 开发者注意事项

- 这是面向Windows UCRT环境的社区驱动适配
- 原始iperf3的版权和许可条款适用于基础代码
- UCRT特定修改在相同BSD许可证下提供
- 此版本专为嵌入式Windows应用开发而设计
- 使用此代码时请确保遵守原始iperf3许可条款

## 社区支持

### 📞 联系方式
- **GitHub Issues**: [https://github.com/Yundi339/iperf-msys/issues](https://github.com/Yundi339/iperf-msys/issues)
- **原始iperf3许可问题**: TTD@lbl.gov

### 🌟 Star History

[![Star History Chart](https://api.star-history.com/svg?repos=Yundi339/iperf-msys&type=Date)](https://www.star-history.com/#Yundi339/iperf-msys&Date)

---

**iperf3-msys2 - Windows UCRT网络性能测试工具**

<div align="center">

**维护者**: [Yundi339](https://github.com/Yundi339) | **项目地址**: [https://github.com/Yundi339/iperf-msys](https://github.com/Yundi339/iperf-msys)

</div>
