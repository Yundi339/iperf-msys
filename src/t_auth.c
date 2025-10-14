/*
 * iperf, Copyright (c) 2020, The Regents of the University of
 * California, through Lawrence Berkeley National Laboratory (subject
 * to receipt of any required approvals from the U.S. Dept. of
 * Energy).  All rights reserved.
 *
 * If you have questions about your rights to use or distribute this
 * software, please contact Berkeley Lab's Technology Transfer
 * Department at TTD@lbl.gov.
 *
 * NOTICE.  This software is owned by the U.S. Department of Energy.
 * As such, the U.S. Government has been granted for itself and others
 * acting on its behalf a paid-up, nonexclusive, irrevocable,
 * worldwide license in the Software to reproduce, prepare derivative
 * works, and perform publicly and display publicly.  Beginning five
 * (5) years after the date permission to assert copyright is obtained
 * from the U.S. Department of Energy, and subject to any subsequent
 * five (5) year renewals, the U.S. Government is granted for itself
 * and others acting on its behalf a paid-up, nonexclusive,
 * irrevocable, worldwide license in the Software to reproduce,
 * prepare derivative works, distribute copies to the public, perform
 * publicly and display publicly, and to permit others to do so.
 *
 * This code is distributed under a BSD style license, see the LICENSE
 * file for complete information.
 */
#include "iperf_config.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "iperf.h"
#include "iperf_api.h"
#if defined(HAVE_SSL)
#include "iperf_auth.h"
#endif /* HAVE_SSL */

#include "version.h"

#include "units.h"


#if defined(HAVE_SSL)
int test_authtoken(const char *authUser, const char *authPassword, EVP_PKEY *pubkey, EVP_PKEY *privkey);

int
main(int argc, char **argv)
{
    printf("=== t_auth 测试开始 ===\n");
    printf("OpenSSL 初始化检查...\n");
    
    /* sha256 */
    void sha256(const char *string, char outputBuffer[65]);
    const char sha256String[] = "This is a SHA256 test.";
    const char sha256Digest[] = "4816482f8b4149f687a1a33d61a0de6b611364ec0fb7adffa59ff2af672f7232"; /* echo -n "This is a SHA256 test." | shasum -a256 */
    char sha256Output[65];

    printf("测试 SHA256 功能...\n");
    sha256(sha256String, sha256Output);
    assert(strcmp(sha256Output, sha256Digest) == 0);
    printf("SHA256 测试通过\n");

    /* Base64{Encode,Decode} */
    int Base64Encode(const unsigned char* buffer, const size_t length, char** b64text);
    int Base64Decode(const char* b64message, unsigned char** buffer, size_t* length);
    const char base64String[] = "This is a Base64 test.";
    char *base64Text;
    char *base64Decode;
    size_t base64DecodeLength;
    const char base64EncodeCheck[] = "VGhpcyBpcyBhIEJhc2U2NCB0ZXN0Lg=="; /* echo -n "This is a Base64 test." | b64encode -r - */

    printf("测试 Base64 编码/解码功能...\n");
    assert(Base64Encode((unsigned char *) base64String, strlen(base64String), &base64Text) == 0);
    assert(strcmp(base64Text, base64EncodeCheck) == 0);
    assert(Base64Decode(base64Text, (unsigned char **) &base64Decode, &base64DecodeLength) == 0);
    assert(strcmp(base64String, base64Decode) == 0);
    printf("Base64 测试通过\n");

    /* public/private key tests */
    const char *pubkeyfile = "public.pem";
    const char *privkeyfile = "private.pem";

    printf("测试密钥文件加载...\n");
    printf("检查密钥文件: %s 和 %s\n", pubkeyfile, privkeyfile);
    
    /* built-in tests */
    printf("测试内置密钥加载函数...\n");
    assert(test_load_pubkey_from_file(pubkeyfile) == 0);
    assert(test_load_private_key_from_file(privkeyfile) == 0);
    printf("内置密钥加载测试通过\n");

    /* load public key pair for use in further tests */
    printf("加载公钥和私钥...\n");
    EVP_PKEY *pubkey = NULL, *privkey = NULL;
    pubkey = load_pubkey_from_file(pubkeyfile);
    assert(pubkey);
    printf("公钥加载成功\n");
    privkey = load_privkey_from_file(privkeyfile);
    assert(privkey);
    printf("私钥加载成功\n");

    /* authentication token tests */
    printf("开始认证令牌测试...\n");
    assert(test_authtoken("kilroy", "fubar", pubkey, privkey) == 0);
    printf("认证令牌测试通过\n");

    printf("清理资源...\n");
    if (pubkey != NULL){
        EVP_PKEY_free(pubkey);
        pubkey = NULL;
    }
    if (privkey != NULL){
        EVP_PKEY_free(privkey);
        privkey = NULL;
    }
    if (base64Text != NULL){
        free(base64Text);
        base64Text = NULL;
    }
    if (base64Decode != NULL){
        free(base64Decode);
        base64Decode = NULL;
    }

    /* This should fail because the data is way too long for the RSA key */
    /* assert(test_authtoken("kilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroykilroy", "fubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubarfubar", pubkey, privkey) < 0); */

    printf("=== t_auth 测试完成 ===\n");
    return 0;
}

int
test_authtoken(const char *authUser, const char *authPassword, EVP_PKEY *pubkey, EVP_PKEY *privkey) {
    printf("  test_authtoken: 开始认证令牌测试\n");
    printf("  test_authtoken: 用户=%s, 密码=%s\n", authUser, authPassword);
    
    char *authToken;
    char *decodeUser;
    char *decodePassword;
    time_t decodeTime;

    int use_pkcs1_padding = 1;
    printf("  test_authtoken: 开始编码认证设置...\n");
    assert(encode_auth_setting(authUser, authPassword, pubkey, &authToken, use_pkcs1_padding) == 0);
    printf("  test_authtoken: 编码成功\n");
    
    printf("  test_authtoken: 开始解码认证设置...\n");
    assert(decode_auth_setting(0, authToken, privkey, &decodeUser, &decodePassword, &decodeTime, use_pkcs1_padding) == 0);
    printf("  test_authtoken: 解码成功\n");

    printf("  test_authtoken: 验证解码结果...\n");
    assert(strcmp(decodeUser, authUser) == 0);
    assert(strcmp(decodePassword, authPassword) == 0);
    printf("  test_authtoken: 解码结果验证通过\n");

    printf("  test_authtoken: 清理内存...\n");
    if (authToken !=NULL){
        free(authToken);
        authToken = NULL;
    }
    if (decodeUser !=NULL){
        free(decodeUser);
        decodeUser = NULL;
    }
    if (decodePassword !=NULL){
        free(decodePassword);
        decodePassword = NULL;
    }

    printf("  test_authtoken: 验证时间戳...\n");
    time_t now = time(NULL);

    assert(now - decodeTime >= 0); /* time has to go forwards */
    assert(now - decodeTime <= 1); /* shouldn't take more than a second to run */
    printf("  test_authtoken: 时间戳验证通过\n");

    printf("  test_authtoken: 认证令牌测试完成\n");
    return 0;
}
#else
int
main(int argc, char **argv)
{
    return 0;
}
#endif /* HAVE_SSL */
