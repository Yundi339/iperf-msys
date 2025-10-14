#!/bin/bash

test_name=$1
shift

echo "运行测试: $test_name"

if ./$test_name "$@"; then
    echo "PASS: $test_name"
    exit 0
else
    exit_status=$?
    echo "FAIL: $test_name"
    echo "=== 测试失败详情 ==="
    echo "测试名称: $test_name"
    echo "退出状态: $exit_status"
    echo "系统信息: $(uname -a)"
    echo "当前目录: $(pwd)"
    echo "时间: $(date)"
    
    # 如果是认证测试失败，显示额外的调试信息
    if [ "$test_name" = "t_auth" ]; then
        echo "=== 认证测试特殊调试信息 ==="
        echo "OpenSSL版本: $(openssl version 2>/dev/null || echo 'OpenSSL未安装')"
        echo "密钥文件检查:"
        if [ -f "public.pem" ]; then
            echo "  ✓ public.pem 存在 ($(stat -c%s public.pem 2>/dev/null || echo 'unknown') 字节)"
        else
            echo "  ✗ public.pem 不存在"
        fi
        if [ -f "private.pem" ]; then
            echo "  ✓ private.pem 存在 ($(stat -c%s private.pem 2>/dev/null || echo 'unknown') 字节)"
        else
            echo "  ✗ private.pem 不存在"
        fi
        if [ -f "iperf_config.h" ]; then
            if grep -q "HAVE_SSL" iperf_config.h 2>/dev/null; then
                echo "  ✓ HAVE_SSL 已定义"
            else
                echo "  ✗ HAVE_SSL 未定义"
            fi
        else
            echo "  ✗ iperf_config.h 不存在"
        fi
    fi
    
    exit 1
fi
