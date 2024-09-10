#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "tcp.hpp"

static bool isRun = true;

static pthread_mutex_t mtx;
static pthread_cond_t cond;

void *echoServer(void *param){
    if (isRun == false) return nullptr;
    unsigned char buffer[1024];
    size_t sz = 0;
    int ret = 0;
    TCP::SERVER_EVENT_t event;
    TCP *obj = (TCP *) param;
    if (obj->serverInit() != 0){
        std::cerr << "Failed to initialize server" << std::endl;
    }
    pthread_mutex_lock(&mtx);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mtx);
    do {
        event = obj->serverEventCheck(125);
        if (event == TCP::EVENT_CONNECT_REQUEST){
            std::cout << "New connection request from client" << std::endl;
            obj->serverAccept();
        }
        else if (event == TCP::EVENT_BYTES_AVAILABLE){
            std::cout << "New bytes available from client" << std::endl;
            if (obj->receiveData() == 0){
                std::cout << "Data Received" << std::endl;
                obj->sendData(obj->getBufferAsVector());
            }
        }
        else if (event == TCP::EVENT_CLIENT_DISCONNECTED){
            std::cout << "A client disconnected from server" << std::endl;
        }
    } while (isRun);
    return nullptr;
}

class TCPSimpleTest:public::testing::Test {
protected:
    TCP server;
    TCP client;
    pthread_t th;
    TCPSimpleTest() : server("127.0.0.1", 4431) {}
    void SetUp() override {
        isRun = true;
        server.setTimeout(250);
        server.setKeepAliveMs(25);
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

/* Default constructor */

TEST_F(TCPSimpleTest, DefaultConstructor_1) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    ASSERT_EQ(client.getAddress(), std::string("127.0.0.1"));
    ASSERT_EQ(client.getPort(), 3000);
    ASSERT_EQ(client.getMaximumClient(), 10);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(client.getKeepAliveMs(), 0);
    ASSERT_EQ(client.getIsUseSSL(), false);
    ASSERT_EQ(client.getSSLVerifyMode(), false);
    ASSERT_EQ(client.getDataSize(), 0);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, CustomConstructor_1) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    TCP cli((const unsigned char *) "\x08\x08\x08\x08");
    ASSERT_EQ(cli.getAddress(), std::string("8.8.8.8"));
    ASSERT_EQ(cli.getPort(), 3000);
    ASSERT_EQ(cli.getMaximumClient(), 10);
    memcpy(&tv, cli.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(cli.getKeepAliveMs(), 0);
    ASSERT_EQ(cli.getIsUseSSL(), false);
    ASSERT_EQ(cli.getSSLVerifyMode(), false);
    ASSERT_EQ(cli.getDataSize(), 0);
    ASSERT_EQ(cli.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(cli.getRemainingDataSize(), 0);
    ASSERT_EQ(cli.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, CustomConstructor_2) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    TCP cli((const unsigned char *) "\x08\x08\x08\x08", 4431);
    ASSERT_EQ(cli.getAddress(), std::string("8.8.8.8"));
    ASSERT_EQ(cli.getPort(), 4431);
    ASSERT_EQ(cli.getMaximumClient(), 10);
    memcpy(&tv, cli.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(cli.getKeepAliveMs(), 0);
    ASSERT_EQ(cli.getIsUseSSL(), false);
    ASSERT_EQ(cli.getSSLVerifyMode(), false);
    ASSERT_EQ(cli.getDataSize(), 0);
    ASSERT_EQ(cli.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(cli.getRemainingDataSize(), 0);
    ASSERT_EQ(cli.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, CustomConstructor_3) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    tmp.push_back(192);
    tmp.push_back(168);
    tmp.push_back(1);
    tmp.push_back(1);
    TCP cli(tmp);
    ASSERT_EQ(cli.getAddress(), std::string("192.168.1.1"));
    ASSERT_EQ(cli.getPort(), 3000);
    ASSERT_EQ(cli.getMaximumClient(), 10);
    memcpy(&tv, cli.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(cli.getKeepAliveMs(), 0);
    ASSERT_EQ(cli.getIsUseSSL(), false);
    ASSERT_EQ(cli.getSSLVerifyMode(), false);
    ASSERT_EQ(cli.getDataSize(), 0);
    ASSERT_EQ(cli.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(cli.getRemainingDataSize(), 0);
    ASSERT_EQ(cli.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, CustomConstructor_4) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    tmp.push_back(192);
    tmp.push_back(168);
    tmp.push_back(1);
    tmp.push_back(1);
    TCP cli(tmp, 4431);
    ASSERT_EQ(cli.getAddress(), std::string("192.168.1.1"));
    ASSERT_EQ(cli.getPort(), 4431);
    ASSERT_EQ(cli.getMaximumClient(), 10);
    memcpy(&tv, cli.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(cli.getKeepAliveMs(), 0);
    ASSERT_EQ(cli.getIsUseSSL(), false);
    ASSERT_EQ(cli.getSSLVerifyMode(), false);
    ASSERT_EQ(cli.getDataSize(), 0);
    ASSERT_EQ(cli.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(cli.getRemainingDataSize(), 0);
    ASSERT_EQ(cli.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, CustomConstructor_5) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    TCP cli("1.1.1.1");
    ASSERT_EQ(cli.getAddress(), std::string("1.1.1.1"));
    ASSERT_EQ(cli.getPort(), 3000);
    ASSERT_EQ(cli.getMaximumClient(), 10);
    memcpy(&tv, cli.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(cli.getKeepAliveMs(), 0);
    ASSERT_EQ(cli.getIsUseSSL(), false);
    ASSERT_EQ(cli.getSSLVerifyMode(), false);
    ASSERT_EQ(cli.getDataSize(), 0);
    ASSERT_EQ(cli.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(cli.getRemainingDataSize(), 0);
    ASSERT_EQ(cli.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, CustomConstructor_6) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    TCP cli("1.1.1.1", 4431);
    ASSERT_EQ(cli.getAddress(), std::string("1.1.1.1"));
    ASSERT_EQ(cli.getPort(), 4431);
    ASSERT_EQ(cli.getMaximumClient(), 10);
    memcpy(&tv, cli.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(cli.getKeepAliveMs(), 0);
    ASSERT_EQ(cli.getIsUseSSL(), false);
    ASSERT_EQ(cli.getSSLVerifyMode(), false);
    ASSERT_EQ(cli.getDataSize(), 0);
    ASSERT_EQ(cli.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(cli.getRemainingDataSize(), 0);
    ASSERT_EQ(cli.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, CustomConstructor_7) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    TCP cli(std::string("0.0.0.0"));
    ASSERT_EQ(cli.getAddress(), std::string("0.0.0.0"));
    ASSERT_EQ(cli.getPort(), 3000);
    ASSERT_EQ(cli.getMaximumClient(), 10);
    memcpy(&tv, cli.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(cli.getKeepAliveMs(), 0);
    ASSERT_EQ(cli.getIsUseSSL(), false);
    ASSERT_EQ(cli.getSSLVerifyMode(), false);
    ASSERT_EQ(cli.getDataSize(), 0);
    ASSERT_EQ(cli.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(cli.getRemainingDataSize(), 0);
    ASSERT_EQ(cli.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, CustomConstructor_8) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    TCP cli(std::string("0.0.0.0"), 4431);
    ASSERT_EQ(cli.getAddress(), std::string("0.0.0.0"));
    ASSERT_EQ(cli.getPort(), 4431);
    ASSERT_EQ(cli.getMaximumClient(), 10);
    memcpy(&tv, cli.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(cli.getKeepAliveMs(), 0);
    ASSERT_EQ(cli.getIsUseSSL(), false);
    ASSERT_EQ(cli.getSSLVerifyMode(), false);
    ASSERT_EQ(cli.getDataSize(), 0);
    ASSERT_EQ(cli.getBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    ASSERT_EQ(cli.getRemainingDataSize(), 0);
    ASSERT_EQ(cli.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(cli.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, ipValidation_1) {
    char ipAddress[16];
    int lsb = 0;
    for (unsigned short i = 0; i <= 255; i+=25){
        for (unsigned short j = 0; j <= 255; j+=25){
            for (unsigned short k = 0; k <= 255; k+=5){
                for (lsb = 0; lsb <= 255; lsb++){
                    memset(ipAddress, 0x00, sizeof(ipAddress));
                    sprintf(ipAddress, "%d.%d.%d.%d", i, j, k, lsb);
                    ASSERT_EQ(client.isValidIPAddress(ipAddress), (lsb == 255 ? false : true));
                }
            }
        }
    }
    ASSERT_EQ(client.isValidIPAddress("255.255.255.255"), false);
}

TEST_F(TCPSimpleTest, ipValidation_2) {
    unsigned char ipAddress[4];
    int lsb = 0;
    for (unsigned short i = 0; i <= 255; i+=25){
        ipAddress[0] = static_cast<unsigned char>(i);
        for (unsigned short j = 0; j <= 255; j+=25){
            ipAddress[1] = static_cast<unsigned char>(j);
            for (unsigned short k = 0; k <= 255; k+=5){
                ipAddress[2] = static_cast<unsigned char>(k);
                for (lsb = 0; lsb <= 255; lsb++){
                    ipAddress[3] = static_cast<unsigned char>(lsb);
                    ASSERT_EQ(client.isValidIPAddress(ipAddress), (lsb == 255 ? false : true));
                }
            }
        }
    }
    ipAddress[2] = 255;
    ASSERT_EQ(client.isValidIPAddress(ipAddress), false);
}

TEST_F(TCPSimpleTest, setterAndGetterTest_1) {
    std::vector <unsigned char> tmp;
    tmp.push_back(1);
    tmp.push_back(1);
    tmp.push_back(1);
    tmp.push_back(1);
    ASSERT_EQ(client.setAddress((const unsigned char *) "\x08\x08\x08\x08"), true);
    ASSERT_EQ(client.getAddress(), "8.8.8.8");
    ASSERT_EQ(client.setAddress(tmp), true);
    ASSERT_EQ(client.getAddress(), "1.1.1.1");
    ASSERT_EQ(client.setAddress("192.168.1.1"), true);
    ASSERT_EQ(client.getAddress(), "192.168.1.1");
    ASSERT_EQ(client.setAddress(std::string("192.147.1.1")), true);
    ASSERT_EQ(client.getAddress(), "192.147.1.1");
    ASSERT_EQ(client.setAddress((const unsigned char *) "\x08\x08\x08\xFF"), false);
    ASSERT_EQ(client.getAddress(), "192.147.1.1");
    tmp.push_back(2);
    ASSERT_EQ(client.setAddress(tmp), false);
    ASSERT_EQ(client.getAddress(), "192.147.1.1");
    ASSERT_EQ(client.setAddress("192.168.1.255"), false);
    ASSERT_EQ(client.getAddress(), "192.147.1.1");
    ASSERT_EQ(client.setAddress(std::string("192.168.147.255")), false);
    ASSERT_EQ(client.getAddress(), "192.147.1.1");
}

TEST_F(TCPSimpleTest, setterAndGetterTest_2) {
    ASSERT_EQ(client.setPort(0), false);
    ASSERT_EQ(client.getPort(), 3000);
    ASSERT_EQ(client.setPort(1), true);
    ASSERT_EQ(client.getPort(), 1);
    ASSERT_EQ(client.setPort(256), true);
    ASSERT_EQ(client.getPort(), 256);
    ASSERT_EQ(client.setPort(0x1FFF), true);
    ASSERT_EQ(client.getPort(), 0x1FFF);
    ASSERT_EQ(client.setPort(0xFFFF), true);
    ASSERT_EQ(client.getPort(), 0xFFFF);
    ASSERT_EQ(client.setPort(0xFFFF + 1), false);
    ASSERT_EQ(client.getPort(), 0xFFFF);
    ASSERT_EQ(client.setPort(-1), false);
    ASSERT_EQ(client.getPort(), 0xFFFF);
}

TEST_F(TCPSimpleTest, setterAndGetterTest_3) {
    ASSERT_EQ(client.setMaximumClient(0), false);
    ASSERT_EQ(client.getMaximumClient(), 10);
    ASSERT_EQ(client.setMaximumClient(1), true);
    ASSERT_EQ(client.getMaximumClient(), 1);
    ASSERT_EQ(client.setMaximumClient(256), true);
    ASSERT_EQ(client.getMaximumClient(), 256);
    ASSERT_EQ(client.setMaximumClient(0x1FFF), true);
    ASSERT_EQ(client.getMaximumClient(), 0x1FFF);
    ASSERT_EQ(client.setMaximumClient(0xFFFF), true);
    ASSERT_EQ(client.getMaximumClient(), 0xFFFF);
    ASSERT_EQ(client.setMaximumClient(0xFFFF + 1), false);
    ASSERT_EQ(client.getMaximumClient(), 0xFFFF);
    ASSERT_EQ(client.setMaximumClient(-1), false);
    ASSERT_EQ(client.getMaximumClient(), 0xFFFF);
}

TEST_F(TCPSimpleTest, setterAndGetterTest_4) {
    struct timeval tv;
    ASSERT_EQ(client.setTimeout(-1), false);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 500000);
    ASSERT_EQ(client.setTimeout(0), true);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 0);
    ASSERT_EQ(tv.tv_usec, 0);
    ASSERT_EQ(client.setTimeout(3725), true);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 3);
    ASSERT_EQ(tv.tv_usec, 725000);
    ASSERT_EQ(client.setTimeout(1, 0), true);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 1);
    ASSERT_EQ(tv.tv_usec, 0);
    ASSERT_EQ(client.setTimeout(9, 125), true);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 9);
    ASSERT_EQ(tv.tv_usec, 125000);
    ASSERT_EQ(client.setTimeout(-1, 125), false);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 9);
    ASSERT_EQ(tv.tv_usec, 125000);
    ASSERT_EQ(client.setTimeout(-1, -1), false);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 9);
    ASSERT_EQ(tv.tv_usec, 125000);
    ASSERT_EQ(client.setTimeout(1, -1), false);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 9);
    ASSERT_EQ(tv.tv_usec, 125000);
    ASSERT_EQ(client.setTimeout(1, 1000), false);
    memcpy(&tv, client.getTimeout(), sizeof(tv));
    ASSERT_EQ(tv.tv_sec, 9);
    ASSERT_EQ(tv.tv_usec, 125000);
}

TEST_F(TCPSimpleTest, setterAndGetterTest_5) {
    ASSERT_EQ(client.setKeepAliveMs(0), true);
    ASSERT_EQ(client.getKeepAliveMs(), 0);
    ASSERT_EQ(client.setKeepAliveMs(0xFFFF), true);
    ASSERT_EQ(client.getKeepAliveMs(), 0xFFFF);
    ASSERT_EQ(client.setKeepAliveMs(-1), false);
    ASSERT_EQ(client.getKeepAliveMs(), 0xFFFF);
}

TEST_F(TCPSimpleTest, setterAndGetterTest_6) {
#ifdef __STCP_SSL__
    ASSERT_EQ(client.setIsUseSSL(true), true);
    ASSERT_EQ(client.getIsUseSSL(), true);
    ASSERT_EQ(client.getSSLVerifyMode(), false);
    ASSERT_EQ(client.setSSLVerifyMode(true), true);
    ASSERT_EQ(client.getSSLVerifyMode(), true);
    ASSERT_EQ(client.setIsUseSSL(false), true);
    ASSERT_EQ(client.getIsUseSSL(), false);
    ASSERT_EQ(client.getSSLVerifyMode(), false);
    ASSERT_EQ(client.setSSLVerifyMode(true), false);
    ASSERT_EQ(client.getSSLVerifyMode(), false);
#else
    ASSERT_EQ(client.setIsUseSSL(true), false);
    ASSERT_EQ(client.getIsUseSSL(), false);
    ASSERT_EQ(client.getSSLVerifyMode(), false);
    ASSERT_EQ(client.setSSLVerifyMode(true), false);
    ASSERT_EQ(client.getSSLVerifyMode(), false);
    ASSERT_EQ(client.setIsUseSSL(false), false);
    ASSERT_EQ(client.getIsUseSSL(), false);
    ASSERT_EQ(client.getSSLVerifyMode(), false);
    ASSERT_EQ(client.setSSLVerifyMode(true), false);
    ASSERT_EQ(client.getSSLVerifyMode(), false);
#endif
}

TEST_F(TCPSimpleTest, communicationTest_1) {
    unsigned char buffer[16];
    std::vector <unsigned char> tmp;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.clientInit(), 0);
    ASSERT_EQ(client.sendData("TCP::EchoTest"), 0);
    ASSERT_EQ(client.receiveData(), 0);
    client.closeSocket();
    ASSERT_EQ(client.getDataSize(), 13);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 13);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "TCP::EchoTest", 13), 0);
    ASSERT_EQ(client.getBuffer(tmp), 13);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "TCP::EchoTest", 13), 0);
    tmp = client.getBufferAsVector();
    ASSERT_EQ(tmp.size(), 13);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "TCP::EchoTest", 13), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    tmp.clear();
    tmp = client.getRemainingBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, communicationTest_2) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.clientInit(), 0);
    ASSERT_EQ(client.sendData("TCP::EchoTest"), 0);
    ASSERT_EQ(client.receiveData(), 0);
    client.closeSocket();
    ASSERT_EQ(client.getDataSize(), 13);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 8);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "TCP::Ech", 8), 0);
    ASSERT_EQ(client.getBuffer(tmp), 13);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "TCP::EchoTest", 13), 0);
    tmp = client.getBufferAsVector();
    ASSERT_EQ(tmp.size(), 13);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "TCP::EchoTest", 13), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
    tmp.clear();
    tmp = client.getRemainingBufferAsVector();
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPSimpleTest, communicationTest_3) {
    unsigned char buffer[16];
    std::vector <unsigned char> tmp;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.clientInit(), 0);
    for (int i = 0; i < 5; i++){
        ASSERT_EQ(client.sendData("TCP::EchoTest"), 0);
        ASSERT_EQ(client.receiveData(), 0);
        ASSERT_EQ(client.getDataSize(), 13);
        ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 13);
        ASSERT_EQ(memcmp(buffer, (const unsigned char *) "TCP::EchoTest", 13), 0);
        ASSERT_EQ(client.getBuffer(tmp), 13);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "TCP::EchoTest", 13), 0);
        tmp = client.getBufferAsVector();
        ASSERT_EQ(tmp.size(), 13);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "TCP::EchoTest", 13), 0);
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
    ASSERT_EQ(client.sendData("TCP::EchoTest"), 0);
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

TEST_F(TCPSimpleTest, communicationTest_4) {
    unsigned char buffer[64];
    std::string sdata = "TCP::EchoTestTCP::EchoTestTCP::EchoTestTCP::EchoTestTCP::EchoTest";
    std::vector <unsigned char> tmp;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.clientInit(), 0);
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