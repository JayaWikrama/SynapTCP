#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "tcp-client.hpp"
#include "tcp-server.hpp"

static bool isRun = true;

static pthread_mutex_t mtx;
static pthread_cond_t cond;

const char *TEST_STR_1 = "TCP::EchoTest";
const char *TEST_STR_2 = "qwerty1234567890";
const char *TEST_STR_3 = "qwertyuiopasdfghjklzxcvbnm09876543210987654321poiuytrewqlkjhgfdsamnbvcxz    \n"
                         "qwertyuiopasdfghjklzxcvbnm09876543210987654321poiuytrewqlkjhgfdsamnbvcxz    \n"
                         "qwertyuiopasdfghjklzxcvbnm09876543210987654321poiuytrewqlkjhgfdsamnbvcxz    \n"
                         "qwertyuiopasdfghjklzxcvbnm09876543210987654321poiuytrewqlkjhgfdsamnbvcxz    \n"
                         "qwertyuiopasdfghjklzxcvbnm09876543210987654321poiuytrewqlkjhgfdsamnbvcxz-1234"
                         "qwertyuiopasdfghjklzxcvbnm09876543210987654321poiuytrewqlkjhgfdsamnbvcxz1234\n";
const char *TEST_STR_4 = "1234567890qwerty";

void receptionCallbackFunction(SynapSock &connection, void *param){
    if (connection.receiveData() == 0){
        std::cout << "Data Received form " << connection.getAddress() << ":" << connection.getPort() << std::endl;
        connection.sendData(connection.getBufferAsVector());
    }
}

void receptionCallbackFunctionEchoDelay(SynapSock &connection, void *param){
    if (connection.receiveData() == 0){
        std::vector <unsigned char> data;
        int mdelay = (int) (long) param;
        std::cout << "Data Received form " << connection.getAddress() << ":" << connection.getPort() << std::endl;
        data = connection.getBufferAsVector();
        for (int i = 0; i < data.size(); i++){
            connection.sendData(data.data() + i, 1);
            usleep(1000 * mdelay);
        }
    }
}

void *echoServer(void *param){
    if (isRun == false) return nullptr;
    unsigned char buffer[1024];
    size_t sz = 0;
    int ret = 0;
    TCPServer::SERVER_EVENT_t event;
    TCPServer *obj = (TCPServer *) param;
    if (obj->init() != 0){
        std::cerr << "Failed to initialize server" << std::endl;
    }
    pthread_mutex_lock(&mtx);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mtx);
    do {
        event = obj->eventCheck(125);
        if (event == TCPServer::EVENT_CONNECT_REQUEST){
            std::cout << "New client accepted" << std::endl;
        }
        else if (event == TCPServer::EVENT_CLIENT_DISCONNECTED){
            std::cout << "A client disconnected from server" << std::endl;
        }
    } while (isRun);
    return nullptr;
}

class TCPSimpleTest:public::testing::Test {
protected:
    TCPServer server;
    TCPClient client;
    pthread_t th;
    TCPSimpleTest() : server("127.0.0.1", 4431) {}
    void SetUp() override {
        isRun = true;
        server.setTimeout(250);
        server.setKeepAliveMs(25);
        server.setReceptionHandler(&receptionCallbackFunction, nullptr);
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

TEST_F(TCPSimpleTest, DefaultConstructor_1) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    struct timeval tv;
    ASSERT_EQ(client.getAddress(), std::string("127.0.0.1"));
    ASSERT_EQ(client.getPort(), 3000);
    ASSERT_EQ(server.getMaximumClient(), 10);
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
    TCPClient cli((const unsigned char *) "\x08\x08\x08\x08");
    ASSERT_EQ(cli.getAddress(), std::string("8.8.8.8"));
    ASSERT_EQ(cli.getPort(), 3000);
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
    TCPClient cli((const unsigned char *) "\x08\x08\x08\x08", 4431);
    ASSERT_EQ(cli.getAddress(), std::string("8.8.8.8"));
    ASSERT_EQ(cli.getPort(), 4431);
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
    TCPClient cli(tmp);
    ASSERT_EQ(cli.getAddress(), std::string("192.168.1.1"));
    ASSERT_EQ(cli.getPort(), 3000);
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
    TCPClient cli(tmp, 4431);
    ASSERT_EQ(cli.getAddress(), std::string("192.168.1.1"));
    ASSERT_EQ(cli.getPort(), 4431);
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
    TCPClient cli("1.1.1.1");
    ASSERT_EQ(cli.getAddress(), std::string("1.1.1.1"));
    ASSERT_EQ(cli.getPort(), 3000);
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
    TCPClient cli("1.1.1.1", 4431);
    ASSERT_EQ(cli.getAddress(), std::string("1.1.1.1"));
    ASSERT_EQ(cli.getPort(), 4431);
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
    TCPClient cli(std::string("0.0.0.0"));
    ASSERT_EQ(cli.getAddress(), std::string("0.0.0.0"));
    ASSERT_EQ(cli.getPort(), 3000);
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
    TCPClient cli(std::string("0.0.0.0"), 4431);
    ASSERT_EQ(cli.getAddress(), std::string("0.0.0.0"));
    ASSERT_EQ(cli.getPort(), 4431);
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
    ASSERT_EQ(server.setMaximumClient(0), false);
    ASSERT_EQ(server.getMaximumClient(), 10);
    ASSERT_EQ(server.setMaximumClient(1), true);
    ASSERT_EQ(server.getMaximumClient(), 1);
    ASSERT_EQ(server.setMaximumClient(256), true);
    ASSERT_EQ(server.getMaximumClient(), 256);
    ASSERT_EQ(server.setMaximumClient(0x1FFF), true);
    ASSERT_EQ(server.getMaximumClient(), 0x1FFF);
    ASSERT_EQ(server.setMaximumClient(0xFFFF), true);
    ASSERT_EQ(server.getMaximumClient(), 0xFFFF);
    ASSERT_EQ(server.setMaximumClient(0xFFFF + 1), false);
    ASSERT_EQ(server.getMaximumClient(), 0xFFFF);
    ASSERT_EQ(server.setMaximumClient(-1), false);
    ASSERT_EQ(server.getMaximumClient(), 0xFFFF);
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

TEST_F(TCPSimpleTest, communicationTest_2) {
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

TEST_F(TCPSimpleTest, communicationTest_3) {
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

TEST_F(TCPSimpleTest, communicationTest_4) {
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

TEST_F(TCPSimpleTest, communicationTest_with_delayed_bytes) {
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

TEST_F(TCPSimpleTest, communicationTest_startBytes) {
    unsigned char buffer[8];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData((const unsigned char *) TEST_STR_2, 16), 0);
    ASSERT_EQ(client.receiveStartBytes((const unsigned char *) (TEST_STR_2 + 6), 4), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 4);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_2 + 6), 4), 0);
    ASSERT_EQ(client.getBuffer(tmp), 4);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_2 + 6), 4), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 6);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 6);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_2 + 10), 6), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 6);
    ASSERT_EQ(tmp.size(), 6);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_2 + 10), 6), 0);
}

TEST_F(TCPSimpleTest, communicationTest_startBytes_ov1) {
    char data[5];
    unsigned char buffer[8];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData(TEST_STR_2), 0);
    strncpy(data, TEST_STR_2 + 6, 4);
    data[4] = 0x00;
    ASSERT_EQ(client.receiveStartBytes(data), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 4);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_2 + 6), 4), 0);
    ASSERT_EQ(client.getBuffer(tmp), 4);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_2 + 6), 4), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 6);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 6);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_2 + 10), 6), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 6);
    ASSERT_EQ(tmp.size(), 6);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_2 + 10), 6), 0);
}

TEST_F(TCPSimpleTest, communicationTest_startBytes_ov2) {
    char data[5];
    unsigned char buffer[8];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData(std::string(TEST_STR_2)), 0);
    strncpy(data, TEST_STR_2 + 6, 4);
    data[4] = 0x00;
    ASSERT_EQ(client.receiveStartBytes(std::string(data)), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 4);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_2 + 6), 4), 0);
    ASSERT_EQ(client.getBuffer(tmp), 4);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_2 + 6), 4), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 6);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 6);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_2 + 10), 6), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 6);
    ASSERT_EQ(tmp.size(), 6);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_2 + 10), 6), 0);
}

TEST_F(TCPSimpleTest, communicationTest_startBytes_ov3) {
    const char *data = TEST_STR_2;
    unsigned char buffer[8];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    tmp.assign((const unsigned char *) data, (const unsigned char *) data + strlen(data));
    ASSERT_EQ(client.sendData(tmp), 0);
    tmp.assign((const unsigned char *) data + 6, (const unsigned char *) data + 10);
    ASSERT_EQ(client.receiveStartBytes(tmp), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 4);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_2 + 6), 4), 0);
    ASSERT_EQ(client.getBuffer(tmp), 4);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_2 + 6), 4), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 6);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 6);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_2 + 10), 6), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 6);
    ASSERT_EQ(tmp.size(), 6);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_2 + 10), 6), 0);
}

TEST_F(TCPSimpleTest, communicationTest_rcvUntillStopBytes) {
    unsigned char buffer[512];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData((const unsigned char *) TEST_STR_3, 462), 0);
    ASSERT_EQ(client.receiveUntillStopBytes((const unsigned char *) (TEST_STR_3 + 381), 4), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 385);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 385);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getBuffer(tmp), 385);
    ASSERT_EQ(tmp.size(), 385);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 77);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 77);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 77);
    ASSERT_EQ(tmp.size(), 77);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
}

TEST_F(TCPSimpleTest, communicationTest_rcvUntillStopBytes_ov1) {
    char data[5];
    unsigned char buffer[512];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData(TEST_STR_3), 0);
    strncpy(data, TEST_STR_3 + 381, 4);
    data[4] = 0x00;
    ASSERT_EQ(client.receiveUntillStopBytes(data), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 385);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 385);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getBuffer(tmp), 385);
    ASSERT_EQ(tmp.size(), 385);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 77);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 77);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 77);
    ASSERT_EQ(tmp.size(), 77);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
}

TEST_F(TCPSimpleTest, communicationTest_rcvUntillStopBytes_ov2) {
    char data[5];
    unsigned char buffer[512];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData(std::string(TEST_STR_3)), 0);
    strncpy(data, TEST_STR_3 + 381, 4);
    data[4] = 0x00;
    ASSERT_EQ(client.receiveUntillStopBytes(std::string(data)), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 385);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 385);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getBuffer(tmp), 385);
    ASSERT_EQ(tmp.size(), 385);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 77);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 77);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 77);
    ASSERT_EQ(tmp.size(), 77);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
}

TEST_F(TCPSimpleTest, communicationTest_rcvUntillStopBytes_ov3) {
    const char *data = TEST_STR_3;
    unsigned char buffer[512];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    tmp.assign((const unsigned char *) data, (const unsigned char *) data + strlen(data));
    ASSERT_EQ(client.sendData(tmp), 0);
    tmp.assign((const unsigned char *) data + 381, (const unsigned char *) data + 385);
    ASSERT_EQ(client.receiveUntillStopBytes(tmp), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 385);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 385);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getBuffer(tmp), 385);
    ASSERT_EQ(tmp.size(), 385);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 77);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 77);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 77);
    ASSERT_EQ(tmp.size(), 77);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
}

TEST_F(TCPSimpleTest, communicationTest_rcvStopBytes) {
    unsigned char buffer[16];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData((const unsigned char *) TEST_STR_4, 16), 0);
    ASSERT_EQ(client.receiveStopBytes((const unsigned char *) TEST_STR_4, 4), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 4);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_4, 4), 0);
    ASSERT_EQ(client.getBuffer(tmp), 4);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_4, 4), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 12);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_4 + 4), 12), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_4 + 4), 12), 0);
}

TEST_F(TCPSimpleTest, communicationTest_rcvStopBytes_ov1) {
    char data[5];
    unsigned char buffer[16];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData(TEST_STR_4), 0);
    strncpy(data, TEST_STR_4, 4);
    data[4] = 0x00;
    ASSERT_EQ(client.receiveStopBytes(data), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 4);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_4, 4), 0);
    ASSERT_EQ(client.getBuffer(tmp), 4);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_4, 4), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 12);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_4 + 4), 12), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_4 + 4), 12), 0);
}

TEST_F(TCPSimpleTest, communicationTest_rcvStopBytes_ov2) {
    char data[5];
    unsigned char buffer[16];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData(std::string(TEST_STR_4)), 0);
    strncpy(data, TEST_STR_4, 4);
    data[4] = 0x00;
    ASSERT_EQ(client.receiveStopBytes(std::string(data)), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 4);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_4, 4), 0);
    ASSERT_EQ(client.getBuffer(tmp), 4);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_4, 4), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 12);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_4 + 4), 12), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_4 + 4), 12), 0);
}

TEST_F(TCPSimpleTest, communicationTest_rcvStopBytes_ov3) {
    unsigned char buffer[16];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    tmp.assign((const unsigned char *) TEST_STR_4, (const unsigned char *) TEST_STR_4 + strlen(TEST_STR_4));
    ASSERT_EQ(client.sendData(tmp), 0);
    tmp.assign((const unsigned char *) TEST_STR_4, (const unsigned char *) TEST_STR_4 + 4);
    ASSERT_EQ(client.receiveStopBytes(tmp), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 4);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_4, 4), 0);
    ASSERT_EQ(client.getBuffer(tmp), 4);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_4, 4), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 12);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_4 + 4), 12), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_4 + 4), 12), 0);
}

TEST_F(TCPSimpleTest, communicationTest_rcvNBytes) {
    unsigned char buffer[512];
    pthread_t thread;
    std::vector <unsigned char> tmp;
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    ASSERT_EQ(client.setPort(4431), true);
    ASSERT_EQ(client.setKeepAliveMs(50), true);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData((const unsigned char *) TEST_STR_3, 462), 0);
    ASSERT_EQ(client.receiveNBytes(385), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 50, true);
    ASSERT_EQ(client.getDataSize(), 385);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 385);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getBuffer(tmp), 385);
    ASSERT_EQ(tmp.size(), 385);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) TEST_STR_3, 385), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 77);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 77);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 77);
    ASSERT_EQ(tmp.size(), 77);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) (TEST_STR_3 + 385), 77), 0);
}