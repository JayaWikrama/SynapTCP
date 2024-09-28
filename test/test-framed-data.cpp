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

extern void receptionCallbackFunction(SynapSock &connection, void *param);
extern void receptionCallbackFunctionEchoDelay(SynapSock &connection, void *param);
extern void *echoServer(void *param);

void setupLengthByCommand(DataFrame &frame, void *ptr){
    int data = 0;
    DataFrame *target = frame.getNext();
    SynapSock *obj = (SynapSock *) ptr;
    if (target == nullptr) return;
    frame.getData((unsigned char *) &data, 1);
    if (data == 0x35)
        target->setSize(3);
    else if (data == 0x36)
        target->setSize(2);
    else
        obj->trigInvDataIndicator();
}

void setupLengthByCommand2(DataFrame &frame, void *ptr){
    int data = 0;
    SynapSock *obj = (SynapSock *) ptr;
    DataFrame *target = frame.getNext()->getNext()->getNext()->getNext();
    if (target == nullptr) return;
    frame.getData((unsigned char *) &data, 1);
    if (data > 3){
        target->setSize(data - 3);
    }
    else {
        obj->trigInvDataIndicator();
    }
}

class TCPFramedDataTest:public::testing::Test {
protected:
    TCPServer server;
    TCPClient client;
    pthread_t th;
    TCPFramedDataTest() : server("127.0.0.1", 4431) {}
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

/* Operator Overloading */

TEST_F(TCPFramedDataTest, OperatorOverloading_1) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, 1);
    cmdBytes.setPostExecuteFunction((const void *) &setupLengthByCommand, nullptr);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    client = startBytes + cmdBytes + dataBytes + stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:4]:<<31323334>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:1]:<<>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long) &setupLengthByCommand) + ">>\n"
              "FRAME_TYPE_DATA[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:4]:<<39302D3D>><<exeFunc:0>><<postFunc:0>>\n");
}