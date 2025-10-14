#!/bin/bash

# 测试认证功能的调试脚本
echo "=== iperf3 认证测试调试信息 ==="
echo "当前时间: $(date)"
echo "当前目录: $(pwd)"
echo "系统信息: $(uname -a)"
echo ""

# 检查SSL库
echo "=== SSL库检查 ==="
if command -v openssl >/dev/null 2>&1; then
    echo "OpenSSL版本: $(openssl version)"
else
    echo "错误: OpenSSL未安装"
fi
echo ""

# 检查密钥文件
echo "=== 密钥文件检查 ==="
if [ -f "public.pem" ]; then
    echo "✓ public.pem 存在"
    echo "文件大小: $(stat -c%s public.pem) 字节"
    echo "文件权限: $(ls -l public.pem)"
else
    echo "✗ public.pem 不存在"
fi

if [ -f "private.pem" ]; then
    echo "✓ private.pem 存在"
    echo "文件大小: $(stat -c%s private.pem) 字节"
    echo "文件权限: $(ls -l private.pem)"
else
    echo "✗ private.pem 不存在"
fi
echo ""

# 检查编译配置
echo "=== 编译配置检查 ==="
if [ -f "iperf_config.h" ]; then
    echo "iperf_config.h 存在"
    if grep -q "HAVE_SSL" iperf_config.h; then
        echo "✓ HAVE_SSL 已定义"
    else
        echo "✗ HAVE_SSL 未定义"
    fi
else
    echo "✗ iperf_config.h 不存在"
fi
echo ""

# 检查测试程序
echo "=== 测试程序检查 ==="
if [ -f "t_auth.exe" ]; then
    echo "✓ t_auth.exe 存在"
    echo "文件大小: $(stat -c%s t_auth.exe) 字节"
    echo "文件权限: $(ls -l t_auth.exe)"
    
    # 尝试运行测试程序并捕获输出
    echo ""
    echo "=== 运行测试程序 ==="
    echo "命令: ./t_auth.exe"
    echo "输出:"
    ./t_auth.exe 2>&1
    echo "退出状态: $?"
else
    echo "✗ t_auth.exe 不存在"
fi
echo ""

# 检查环境变量
echo "=== 环境变量检查 ==="
echo "PATH: $PATH"
echo "LD_LIBRARY_PATH: ${LD_LIBRARY_PATH:-未设置}"
echo "PKG_CONFIG_PATH: ${PKG_CONFIG_PATH:-未设置}"
echo ""

echo "=== 调试信息结束 ==="
