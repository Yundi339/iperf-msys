lib_LTLIBRARIES         = libiperf.la                                   # Build and install an iperf library
bin_PROGRAMS            = iperf3                                        # Build and install an iperf binary
if ENABLE_PROFILING
noinst_PROGRAMS         = t_timer t_units t_uuid t_api t_auth iperf3_profile   # Build, but don't install the test programs and a profiled version of iperf3
else
noinst_PROGRAMS         = t_timer t_units t_uuid t_api t_auth                  # Build, but don't install the test programs
endif
include_HEADERS         = iperf_api.h                                   # Defines the headers that get installed with the program


# Specify the source files and flags for the iperf library
libiperf_la_LIBADD      = $(LIBS)
libiperf_la_LDFLAGS     = -version-info 0:0:0
libiperf_la_SOURCES     = \
                        cjson.c \
                        cjson.h \
                        flowlabel.h \
                        iperf.h \
                        iperf_api.c \
                        iperf_api.h \
                        iperf_error.c \
                        iperf_auth.h \
                        iperf_auth.c \
                        iperf_client_api.c \
                        iperf_locale.c \
                        iperf_locale.h \
                        iperf_server_api.c \
                        iperf_tcp.c \
                        iperf_tcp.h \
                        iperf_udp.c \
                        iperf_udp.h \
                        iperf_sctp.c \
                        iperf_sctp.h \
                        iperf_util.c \
                        iperf_util.h \
                        iperf_time.c \
                        iperf_time.h \
                        iperf_pthread.c \
                        iperf_pthread.h \
			dscp.c \
                        net.c \
                        net.h \
                        portable_endian.h \
                        queue.h \
                        tcp_info.c \
                        timer.c \
                        timer.h \
                        units.c \
                        units.h \
                        version.h

# Specify the sources and various flags for the iperf binary
iperf3_SOURCES          = main.c
iperf3_CFLAGS           = -g
iperf3_LDADD            = libiperf.la $(LIBS)
iperf3_LDFLAGS          = -g

if ENABLE_PROFILING
# If the iperf-profiled-binary is enabled
# Specify the sources and various flags for the profiled iperf binary. This
# binary recompiles all the source files to make sure they are all profiled.
iperf3_profile_SOURCES  = main.c \
                          $(libiperf_la_SOURCES)

iperf3_profile_CFLAGS   = -pg -g
iperf3_profile_LDADD    = libiperf.la $(LIBS)
iperf3_profile_LDFLAGS  = -pg -g
endif

# Specify the sources and various flags for the test cases
t_timer_SOURCES         = t_timer.c
t_timer_CFLAGS          = -g
t_timer_LDFLAGS         =
t_timer_LDADD           = libiperf.la $(LIBS)

t_units_SOURCES         = t_units.c
t_units_CFLAGS          = -g
t_units_LDFLAGS         =
t_units_LDADD           = libiperf.la $(LIBS)

t_uuid_SOURCES          = t_uuid.c
t_uuid_CFLAGS           = -g
t_uuid_LDFLAGS          =
t_uuid_LDADD            = libiperf.la $(LIBS)

t_api_SOURCES           = t_api.c
t_api_CFLAGS            = -g
t_api_LDFLAGS           =
t_api_LDADD             = libiperf.la $(LIBS)

t_auth_SOURCES           = t_auth.c
t_auth_CFLAGS            = -g
t_auth_LDFLAGS           =
t_auth_LDADD             = libiperf.la $(LIBS)



# Specify which tests to run during a "make check"
TESTS                   = \
                        t_timer \
                        t_units \
                        t_uuid  \
                        t_api \
			t_auth

# 设置测试环境，使测试失败时显示详细错误信息
AM_TESTS_ENVIRONMENT = \
    export VERBOSE=1; \
    export TEST_DEBUG=1;

# 创建测试包装器脚本
test_wrapper.sh:
	@echo '#!/bin/bash' > test_wrapper.sh
	@echo 'test_name=$$1' >> test_wrapper.sh
	@echo 'shift' >> test_wrapper.sh
	@echo 'echo "运行测试: $$test_name"' >> test_wrapper.sh
	@echo 'if ./$$test_name "$$@"; then' >> test_wrapper.sh
	@echo '    echo "PASS: $$test_name"' >> test_wrapper.sh
	@echo '    exit 0' >> test_wrapper.sh
	@echo 'else' >> test_wrapper.sh
	@echo '    echo "FAIL: $$test_name"' >> test_wrapper.sh
	@echo '    echo "=== 测试失败详情 ==="' >> test_wrapper.sh
	@echo '    echo "测试名称: $$test_name"' >> test_wrapper.sh
	@echo '    echo "退出状态: $$?"' >> test_wrapper.sh
	@echo '    echo "系统信息: $$(uname -a)"' >> test_wrapper.sh
	@echo '    echo "当前目录: $$(pwd)"' >> test_wrapper.sh
	@echo '    echo "时间: $$(date)"' >> test_wrapper.sh
	@echo '    if [ "$$test_name" = "t_auth" ]; then' >> test_wrapper.sh
	@echo '        echo "=== 认证测试特殊调试信息 ==="' >> test_wrapper.sh
	@echo '        echo "OpenSSL版本: $$(openssl version 2>/dev/null || echo '\''OpenSSL未安装'\'')"' >> test_wrapper.sh
	@echo '        echo "密钥文件检查:"' >> test_wrapper.sh
	@echo '        if [ -f "public.pem" ]; then' >> test_wrapper.sh
	@echo '            echo "  ✓ public.pem 存在 ($$(stat -c%s public.pem 2>/dev/null || echo '\''unknown'\'') 字节)"' >> test_wrapper.sh
	@echo '        else' >> test_wrapper.sh
	@echo '            echo "  ✗ public.pem 不存在"' >> test_wrapper.sh
	@echo '        fi' >> test_wrapper.sh
	@echo '        if [ -f "private.pem" ]; then' >> test_wrapper.sh
	@echo '            echo "  ✓ private.pem 存在 ($$(stat -c%s private.pem 2>/dev/null || echo '\''unknown'\'') 字节)"' >> test_wrapper.sh
	@echo '        else' >> test_wrapper.sh
	@echo '            echo "  ✗ private.pem 不存在"' >> test_wrapper.sh
	@echo '        fi' >> test_wrapper.sh
	@echo '        if [ -f "iperf_config.h" ]; then' >> test_wrapper.sh
	@echo '            if grep -q "HAVE_SSL" iperf_config.h 2>/dev/null; then' >> test_wrapper.sh
	@echo '                echo "  ✓ HAVE_SSL 已定义"' >> test_wrapper.sh
	@echo '            else' >> test_wrapper.sh
	@echo '                echo "  ✗ HAVE_SSL 未定义"' >> test_wrapper.sh
	@echo '            fi' >> test_wrapper.sh
	@echo '        else' >> test_wrapper.sh
	@echo '            echo "  ✗ iperf_config.h 不存在"' >> test_wrapper.sh
	@echo '        fi' >> test_wrapper.sh
	@echo '    fi' >> test_wrapper.sh
	@echo '    exit 1' >> test_wrapper.sh
	@echo 'fi' >> test_wrapper.sh
	@chmod +x test_wrapper.sh

# 修改测试目标，使用包装器
check-local: test_wrapper.sh
	@echo "=== 使用增强的测试包装器运行测试 ==="
	@for test in $(TESTS); do \
		echo "运行测试: $$test"; \
		./test_wrapper.sh $$test || exit 1; \
	done
	@echo "=== 所有测试完成 ==="

dist_man_MANS          = iperf3.1 libiperf.3
