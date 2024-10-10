#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "tcp-client.hpp"
#include "tcp-server.hpp"

extern bool isRun;

extern pthread_mutex_t mtx;
extern pthread_cond_t cond;

extern const char *TEST_STR_1;
extern const char *TEST_STR_2;
extern const char *TEST_STR_3;
extern const char *TEST_STR_4;

extern void receptionCallbackFunction(SynapSock &connection, void *param);
extern void receptionCallbackFunctionEchoDelay(SynapSock &connection, void *param);
extern void *echoServer(void *param);

const char *cert = R"(
-----BEGIN CERTIFICATE-----
MIIDyTCCArGgAwIBAgIUVZSJHagX7DUsEneznt53jwyzQR8wDQYJKoZIhvcNAQEL
BQAwdDELMAkGA1UEBhMCSUQxEDAOBgNVBAgMB0pha2FydGExFTATBgNVBAcMDEVh
c3QgSmFrYXJ0YTELMAkGA1UECgwCTkcxDDAKBgNVBAsMA0RldjEMMAoGA1UEAwwD
RGV2MRMwEQYJKoZIhvcNAQkBFgRub25lMB4XDTI0MTAwOTA5MzE1N1oXDTI1MTAw
OTA5MzE1N1owdDELMAkGA1UEBhMCSUQxEDAOBgNVBAgMB0pha2FydGExFTATBgNV
BAcMDEVhc3QgSmFrYXJ0YTELMAkGA1UECgwCTkcxDDAKBgNVBAsMA0RldjEMMAoG
A1UEAwwDRGV2MRMwEQYJKoZIhvcNAQkBFgRub25lMIIBIjANBgkqhkiG9w0BAQEF
AAOCAQ8AMIIBCgKCAQEA07MHS9qpBwhw2Y1Xfoqbp9q00vaL4AzPb6MO4kpn6hlp
8xWs6VBbbqvgIFoGwBPK+T261zwxAKurbJy6sczbE9sNdywObv7Zhoo/zZcl9D/s
Ci7JHeLomGM00UoHxfDr+q/a8D8vPJsajJp+JWvc+veTzRIV3qy8EZizIAvwpP/S
NVgY7tFsP6Qbps8VJgeqEb08XmM5gIygJXKEdJ0W+9F1auZzKqAakk01t35SWMOR
VTSLnRlo6/kAAr60ixN6R0umvvcwHqy/Q/E13L4AHADB5pRVS73m661JXo9Oi2IT
Az4HmtuX6LSOrP8W0HLg2GdWg+AcC+k9xA2PKKzJgwIDAQABo1MwUTAdBgNVHQ4E
FgQUGCJj+Q4PIBSVHgntrBlNfV3b4m4wHwYDVR0jBBgwFoAUGCJj+Q4PIBSVHgnt
rBlNfV3b4m4wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEApr7X
DzyMDsTcDQIRoiBtU8nLlXw0nXZJJnbRN1mVheloZ05gd+fBfQFYOlr8DL3wNQDY
3KaLiXduD4MYlaJ6Ny02HDoOyG43TgzJa3b80u1N1mSSgOQJyI75QLhEBrSNi8YQ
WLmuFLBeoT2p3aWpJ+tTs4kvzSDnR0XI6CAiEyTVrkD5BZWNM0C/8P7S6FzfyN0N
d0Sk+q4j8gy5UgaaxLgqAgKctq3hIBEBv6Hq5c5LPJFMC/lmTuXtxqOu5p8SayJ4
qSDW5PGtZ0PU6nx3VEGPvzOLybFjboBodanRXY7zItuTfUKUtcvGKE60QeP9R1cT
NqTfGscmQcQFCE16rw==
-----END CERTIFICATE-----
)";

const char *key = R"(
-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDTswdL2qkHCHDZ
jVd+ipun2rTS9ovgDM9vow7iSmfqGWnzFazpUFtuq+AgWgbAE8r5PbrXPDEAq6ts
nLqxzNsT2w13LA5u/tmGij/NlyX0P+wKLskd4uiYYzTRSgfF8Ov6r9rwPy88mxqM
mn4la9z695PNEhXerLwRmLMgC/Ck/9I1WBju0Ww/pBumzxUmB6oRvTxeYzmAjKAl
coR0nRb70XVq5nMqoBqSTTW3flJYw5FVNIudGWjr+QACvrSLE3pHS6a+9zAerL9D
8TXcvgAcAMHmlFVLvebrrUlej06LYhMDPgea25fotI6s/xbQcuDYZ1aD4BwL6T3E
DY8orMmDAgMBAAECggEAI83HkiCtLa9TxreM8K3onX7zzio8hewkuTDOt5717DXY
IXCtCdi5XgJbqZ0QPzQZxAWYsAZSE1b1ay0mtiN7eeQ2Zs1gs40OpR6Eo/TISoL1
HNPE1/UkeYFt0We/4M5aWs+bK77D8nAZEWafpxyTBy8QJELGyB5WwHMe+HCMhpse
mZ2X39rYSIREvQVHLVaeu5mQMm0EZ16wAxlslSmxIc+Km5iKGvH47ChOlELwQn9f
JdJGsfphTu8pshs3UDhDTJxGCg2F+f2EwTWoMa5abB0oTXs2HQy5u277qBpKv6ko
Vdkn5lBaF/dRYT77AFyl+EC9Lt8yhMPYA/mHPiabYQKBgQDVOr68vAGmw0KPGCkF
aFyY70FdrIGL2tL9dgDyZhckZiZ6s3Y/Ts3vqEk8A1ND54yTotr3l6o8veKs1hn9
pOA1P/a55qb7W88Oue9Soe3lQZ6SctE1iIJssf9EmnEutaUchbnzjwKVy83JPbMy
BgcV808TnwkUJjT2YgJptGPL3wKBgQD+KbYOvv1oGVTFWC/lXjFNIkrRZD/wP5Fe
pfy116+nHgNh7lW9tan7p+56kX0VJzj8SIklD3q8u5xckYHaNBdJuFohRCMfD9Wu
WelH2k2/JlWE2QchDMxGBhy3pTdHm6E/UB7IlGM1GjCOXCoTLvHKOjYpOEFVmCNm
hWQ5UxN23QKBgGlJIHVAv+//6hWN8HNjHJXHTZdbXnwYCKZuZflKv5KRfd6ITpLj
7nuvtLmVSr/0zUz41PX8RWrIt+xvq1W625pvgvw4D6t9obe7x5gdrMHOrOXTG8o7
JWtWWJ0fRVOzDVwQC8lfi/87Ktl7Rcglgcos4ZbmqCzy0KP3sQlQsRYxAoGAAVl/
Wfyxo8fep12cg39Ii7xO4WXC4Se1QednvB91E1IZen+lfOhmCqwNnCEBpQccSlqo
P8YhRLdKUUOEiQAVz5JnCB4wgY23+MEJeBQ0rXsqoRX177r9qrGFnYQHEx5EcO2f
/Ditfi6xa1AZ+RgauJQujXF7B1VTesviiFtkkxECgYEAgk5+wMohusnGFqs+RFRV
9z6NRXBCgxdBi5lQAOJadVGe9uMpTzktoYFRyLm8mK/m4WtA6ptntnl5FwTyV5zx
vVuzdmaHcmUNcUeIsI0kEx2L55dXiMJNHPTjF3bjN8s+Oyw+PaJ3OrI2BfRYZLAj
ns+PbXHI22oED4oJXRcvvHg=
-----END PRIVATE KEY-----
)";

class SSLSimpleTest:public::testing::Test {
protected:
    TCPServer server;
    TCPClient client;
    pthread_t th;
    SSLSimpleTest() : server("127.0.0.1", 4431) {}
    void SetUp() override {
        isRun = true;
        server.setTimeout(250);
        server.setKeepAliveMs(25);
        server.setReceptionHandler(&receptionCallbackFunction, nullptr);
        server.setIsUseSSL(true);
        server.initializeSSL(std::string(cert), std::string(key));
        client.setIsUseSSL(true);
        client.initializeSSL();
        pthread_mutex_init(&mtx, nullptr);
        pthread_cond_init(&cond, nullptr);
        std::cout << "Create Echo Server Thread" << std::endl;
        pthread_create(&th, nullptr, &echoServer, (void *) &server);
        pthread_mutex_lock(&mtx);
        pthread_cond_wait(&cond, &mtx);
        pthread_mutex_unlock(&mtx);
    }

    void TearDown() override {
        isRun = false;
        pthread_join(th, nullptr);
        std::cout << "Echo Server Thread Stoped" << std::endl;
    }
};

TEST_F(SSLSimpleTest, communicationTest_1) {
    unsigned char buffer[16];
    std::vector <unsigned char> tmp;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData(TEST_STR_1), 0);
    ASSERT_EQ(client.receiveData(), 0);
    client.closeSocket();
    ASSERT_EQ(client.getDataSize(), 13);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 13);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_1, 13), 0);
    ASSERT_EQ(client.getBuffer(tmp), 13);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_1, 13), 0);
    tmp = client.getBufferAsVector();
    ASSERT_EQ(tmp.size(), 13);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_1, 13), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    tmp.clear();
    tmp = client.getRemainingBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(SSLSimpleTest, communicationTest_2) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData(TEST_STR_1), 0);
    ASSERT_EQ(client.receiveData(), 0);
    client.closeSocket();
    ASSERT_EQ(client.getDataSize(), 13);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 8);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_1, 8), 0);
    ASSERT_EQ(client.getBuffer(tmp), 13);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_1, 13), 0);
    tmp = client.getBufferAsVector();
    ASSERT_EQ(tmp.size(), 13);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_1, 13), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    tmp.clear();
    tmp = client.getRemainingBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(SSLSimpleTest, communicationTest_3) {
    unsigned char buffer[16];
    std::vector <unsigned char> tmp;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.init(), 0);
    for (int i = 0; i < 5; i++){
        ASSERT_EQ(client.sendData(TEST_STR_1), 0);
        ASSERT_EQ(client.receiveData(), 0);
        ASSERT_EQ(client.getDataSize(), 13);
        ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 13);
        ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_1, 13), 0);
        ASSERT_EQ(client.getBuffer(tmp), 13);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_1, 13), 0);
        tmp = client.getBufferAsVector();
        ASSERT_EQ(tmp.size(), 13);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_1, 13), 0);
        ASSERT_EQ(client.getRemainingDataSize(), 0);
        ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
        ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
        ASSERT_EQ(tmp.size(), 0);
        tmp.clear();
        tmp = client.getRemainingBufferAsVector();
        ASSERT_EQ(tmp.size(), 0);
        usleep(125000);
    }
    usleep(130000);
    ASSERT_EQ(client.sendData(TEST_STR_1), 0);
    ASSERT_EQ(client.receiveData(), 2);
    client.closeSocket();
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(buffer[0], 0x00);
    ASSERT_EQ(client.getBuffer(tmp), 0);
    tmp = client.getBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    tmp.clear();
    tmp = client.getRemainingBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(SSLSimpleTest, communicationTest_4) {
    unsigned char buffer[64];
    std::string sdata = std::string(TEST_STR_1) + std::string(TEST_STR_1) + std::string(TEST_STR_1) + std::string(TEST_STR_1) + std::string(TEST_STR_1);
    std::vector <unsigned char> tmp;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData(sdata), 0);
    for (int i = 0; i < 5; i++){
        ASSERT_EQ(client.receiveData(13), 0);
        ASSERT_EQ(client.getDataSize(), 13);
        ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 13);
        ASSERT_EQ(memcmp(buffer, (const unsigned char *) sdata.c_str(), 13), 0);
        ASSERT_EQ(client.getBuffer(tmp), 13);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) sdata.c_str(), 13), 0);
        tmp = client.getBufferAsVector();
        ASSERT_EQ(tmp.size(), 13);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) sdata.c_str(), 13), 0);
        ASSERT_EQ(client.getRemainingDataSize(), (sdata.length() - 13 * (i + 1)));
        ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), (sdata.length() - 13 * (i + 1)));
        ASSERT_EQ(memcmp(buffer, (const unsigned char *) sdata.c_str() + 13 * (i + 1), (sdata.length() - 13 * (i + 1))), 0);
        ASSERT_EQ(client.getRemainingBuffer(tmp), (sdata.length() - 13 * (i + 1)));
        ASSERT_EQ(tmp.size(), (sdata.length() - 13 * (i + 1)));
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) sdata.c_str() + 13 * (i + 1), (sdata.length() - 13 * (i + 1))), 0);
        tmp.clear();
        tmp = client.getRemainingBufferAsVector();
        ASSERT_EQ(tmp.size(), (sdata.length() - 13 * (i + 1)));
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) sdata.c_str() + 13 * (i + 1), (sdata.length() - 13 * (i + 1))), 0);
        usleep(125000);
    }
    ASSERT_EQ(client.sendData(sdata), 0);
    ASSERT_EQ(client.receiveData(), 2);
    client.closeSocket();
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(buffer[0], 0x00);
    ASSERT_EQ(client.getBuffer(tmp), 0);
    tmp = client.getBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    tmp.clear();
    tmp = client.getRemainingBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(SSLSimpleTest, communicationTest_with_delayed_bytes) {
    unsigned char buffer[64];
    std::string sdata = std::string(TEST_STR_1) + std::string(TEST_STR_1) + std::string(TEST_STR_1) + std::string(TEST_STR_1) + std::string(TEST_STR_1);
    std::vector <unsigned char> tmp;
    server.setReceptionHandler(&receptionCallbackFunctionEchoDelay, (void *) (long) 50);
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.setKeepAliveMs(75), true);
    ASSERT_EQ(client.sendData(sdata), 0);
    for (int i = 0; i < 5; i++){
        ASSERT_EQ(client.receiveData(13), 0);
        ASSERT_EQ(client.getDataSize(), 13);
        ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 13);
        ASSERT_EQ(memcmp(buffer, (const unsigned char *) sdata.c_str(), 13), 0);
        ASSERT_EQ(client.getBuffer(tmp), 13);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) sdata.c_str(), 13), 0);
        tmp = client.getBufferAsVector();
        ASSERT_EQ(tmp.size(), 13);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) sdata.c_str(), 13), 0);
        usleep(125000);
    }
    ASSERT_EQ(client.sendData(sdata), 0);
    ASSERT_EQ(client.receiveData(), 2);
    client.closeSocket();
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(buffer[0], 0x00);
    ASSERT_EQ(client.getBuffer(tmp), 0);
    tmp = client.getBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    tmp.clear();
    tmp = client.getRemainingBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
}