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

TEST_F(TCPFramedDataTest, OperatorOverloading_2) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, 1);
    cmdBytes.setPostExecuteFunction((const void *) &setupLengthByCommand, nullptr);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    client = startBytes;
    client += cmdBytes;
    client += dataBytes;
    client += stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:4]:<<31323334>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:1]:<<>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long) &setupLengthByCommand) + ">>\n"
              "FRAME_TYPE_DATA[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:4]:<<39302D3D>><<exeFunc:0>><<postFunc:0>>\n");
}

TEST_F(TCPFramedDataTest, OperatorOverloading_3) {
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
    DataFrame *frame = nullptr;
    frame = client[0];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_START_BYTES);
    frame = client[1];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_COMMAND);
    frame = client[2];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_DATA);
    frame = client[3];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    frame = client[4];
    ASSERT_EQ(frame, nullptr);
}

TEST_F(TCPFramedDataTest, OperatorOverloading_4) {
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
    DataFrame *frame = nullptr;
    frame = client[DataFrame::FRAME_TYPE_START_BYTES];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getSize(), 4);
    frame = client[DataFrame::FRAME_TYPE_COMMAND];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getSize(), 1);
    frame = client[DataFrame::FRAME_TYPE_DATA];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getSize(), 0);
    frame = client[DataFrame::FRAME_TYPE_STOP_BYTES];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getSize(), 4);
    frame = client[DataFrame::FRAME_TYPE_VALIDATOR];
    ASSERT_EQ(frame, nullptr);
}

TEST_F(TCPFramedDataTest, OperatorOverloading_5) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame cmdBytes0(DataFrame::FRAME_TYPE_COMMAND, 1);
    DataFrame dataBytes0(DataFrame::FRAME_TYPE_DATA, 1);
    DataFrame cmdBytes1(DataFrame::FRAME_TYPE_COMMAND, 2);
    DataFrame dataBytes1(DataFrame::FRAME_TYPE_DATA, 2);
    DataFrame cmdBytes2(DataFrame::FRAME_TYPE_COMMAND, 3);
    DataFrame dataBytes2(DataFrame::FRAME_TYPE_DATA, 3);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    client = startBytes + cmdBytes0 + dataBytes0 + cmdBytes1 + dataBytes1 + cmdBytes2 + dataBytes2 + stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:4]:<<31323334>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:1]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:1]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:2]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:2]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:3]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:3]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:4]:<<39302D3D>><<exeFunc:0>><<postFunc:0>>\n");
    DataFrame *frame = nullptr;
    frame = client[{DataFrame::FRAME_TYPE_START_BYTES, 0}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_START_BYTES);
    ASSERT_EQ(frame->getSize(), 4);
    frame = client[{DataFrame::FRAME_TYPE_COMMAND, 0}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(frame->getSize(), 1);
    frame = client[{DataFrame::FRAME_TYPE_DATA, 0}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(frame->getSize(), 1);
    frame = client[{DataFrame::FRAME_TYPE_COMMAND, 1}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(frame->getSize(), 2);
    frame = client[{DataFrame::FRAME_TYPE_DATA, 1}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(frame->getSize(), 2);
    frame = client[{DataFrame::FRAME_TYPE_COMMAND, 2}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_COMMAND);
    ASSERT_EQ(frame->getSize(), 3);
    frame = client[{DataFrame::FRAME_TYPE_DATA, 2}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(frame->getSize(), 3);
    frame = client[{DataFrame::FRAME_TYPE_STOP_BYTES}];
    ASSERT_NE(frame, nullptr);
    ASSERT_EQ(frame->getType(), DataFrame::FRAME_TYPE_STOP_BYTES);
    ASSERT_EQ(frame->getSize(), 4);
    frame = client[{DataFrame::FRAME_TYPE_VALIDATOR}];
    ASSERT_EQ(frame, nullptr);
}

TEST_F(TCPFramedDataTest, OperatorOverloading_6) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, "5");
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA, "678");
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    client = startBytes + cmdBytes + dataBytes + stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:4]:<<31323334>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:1]:<<35>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:3]:<<363738>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:4]:<<39302D3D>><<exeFunc:0>><<postFunc:0>>\n");
    tmp = client.getSpecificBufferAsVector(DataFrame::FRAME_TYPE_COMMAND, DataFrame::FRAME_TYPE_DATA);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "5678", 4), 0);
}

TEST_F(TCPFramedDataTest, OperatorOverloading_7) {
    unsigned char buffer[8];
    std::vector <unsigned char> tmp;
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, "5");
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA, "678");
    DataFrame cmdBytes1(DataFrame::FRAME_TYPE_COMMAND, "Q");
    DataFrame dataBytes1(DataFrame::FRAME_TYPE_DATA, "WER");
    DataFrame cmdBytes2(DataFrame::FRAME_TYPE_COMMAND, "A");
    DataFrame dataBytes2(DataFrame::FRAME_TYPE_DATA, "SDF");
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    client = startBytes + cmdBytes + dataBytes + cmdBytes1 + dataBytes1 + cmdBytes2 + dataBytes2 + stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:4]:<<31323334>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:1]:<<35>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:3]:<<363738>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:1]:<<51>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:3]:<<574552>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:1]:<<41>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:3]:<<534446>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:4]:<<39302D3D>><<exeFunc:0>><<postFunc:0>>\n");
    DataFrame *begin = client[{DataFrame::FRAME_TYPE_COMMAND, 1}];
    DataFrame *end = client[{DataFrame::FRAME_TYPE_DATA, 2}];
    tmp = client.getSpecificBufferAsVector(begin, end);
    ASSERT_EQ(tmp.size(), 8);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "QWERASDF", 8), 0);
}

/* Read Test */

TEST_F(TCPFramedDataTest, SendTest_1) {
    unsigned char buffer[16];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, "5");
    cmdBytes.setPostExecuteFunction((const void *) &setupLengthByCommand, nullptr);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA, "678");
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    client = startBytes + cmdBytes + dataBytes + stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:4]:<<31323334>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:1]:<<35>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long) &setupLengthByCommand) + ">>\n"
              "FRAME_TYPE_DATA[size:3]:<<363738>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:4]:<<39302D3D>><<exeFunc:0>><<postFunc:0>>\n");
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendFramedData(), 0);
    ASSERT_EQ(client.receiveFramedData(), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 75, true);
    ASSERT_EQ(client.getDataSize(), 12);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPFramedDataTest, ReceptionTest_1) {
    unsigned char buffer[16];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
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
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData("1234567890-="), 0);
    ASSERT_EQ(client.receiveFramedData(), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 75, true);
    ASSERT_EQ(client.getDataSize(), 12);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPFramedDataTest, ReceptionTest_withSuffix_1) {
    unsigned char buffer[64];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
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
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData("1234567890-=qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm"), 0);
    ASSERT_EQ(client.receiveFramedData(), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 75, true);
    ASSERT_EQ(client.getDataSize(), 12);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 60);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 60);
    ASSERT_EQ(memcmp(buffer, (unsigned char *) "qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm", 60), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 60);
    ASSERT_EQ(tmp.size(), 60);
    ASSERT_EQ(memcmp(tmp.data(), (unsigned char *) "qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm", 60), 0);
}

TEST_F(TCPFramedDataTest, ReceptionTest_withPrefixAndSuffix_1) {
    unsigned char buffer[64];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
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
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData("qwertyuioplkjhgfdsazxcvbnmqwertyuioplkjhgfdsazxcvbnm1234567890-=qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm"), 0);
    ASSERT_EQ(client.receiveFramedData(), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 75, true);
    ASSERT_EQ(client.getDataSize(), 12);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 60);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 60);
    ASSERT_EQ(memcmp(buffer, (unsigned char *) "qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm", 60), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 60);
    ASSERT_EQ(tmp.size(), 60);
    ASSERT_EQ(memcmp(tmp.data(), (unsigned char *) "qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm", 60), 0);
}

TEST_F(TCPFramedDataTest, ReceptionTest_withPrefixAndSuffix_2) {
    unsigned char buffer[64];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
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
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData("m1234567890-=qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm"), 0);
    ASSERT_EQ(client.receiveFramedData(), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 75, true);
    ASSERT_EQ(client.getDataSize(), 12);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 60);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 60);
    ASSERT_EQ(memcmp(buffer, (unsigned char *) "qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm", 60), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 60);
    ASSERT_EQ(tmp.size(), 60);
    ASSERT_EQ(memcmp(tmp.data(), (unsigned char *) "qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm", 60), 0);
}

TEST_F(TCPFramedDataTest, ReceptionTest_withPrefixAndSuffix_3) {
    unsigned char buffer[16];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    int i = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
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
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData("qwertyuiop[1234567890-=zxcvbnmqwertyuiop[1234567890-=zxcvbnmqwertyuiop[1234567890-=zxcvbnmqwertyuiop[1234567890-=zxcvbnm"), 0);
    for (i = 0; i < 4; i++){
        gettimeofday(&tvStart, NULL);
        ASSERT_EQ(client.receiveFramedData(), 0);
        gettimeofday(&tvEnd, NULL);
        diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
        ASSERT_EQ(diffTime <= 75, true);
        ASSERT_EQ(client.getDataSize(), 12);
        ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 12);
        ASSERT_EQ(memcmp(buffer, (const unsigned char *) "1234567890-=", 12), 0);
        ASSERT_EQ(client.getBuffer(tmp), 12);
        ASSERT_EQ(tmp.size(), 12);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "1234567890-=", 12), 0);
    }
    ASSERT_EQ(client.getRemainingDataSize(), 7);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 7);
    ASSERT_EQ(memcmp(buffer, (unsigned char *) "zxcvbnm", 7), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 7);
    ASSERT_EQ(tmp.size(), 7);
    ASSERT_EQ(memcmp(tmp.data(), (unsigned char *) "zxcvbnm", 7), 0);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.receiveFramedData(), 2);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 240 && diffTime <= 280, true);
    ASSERT_EQ(client.getDataSize(), 7);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 7);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "zxcvbnm", 7), 0);
    ASSERT_EQ(client.getBuffer(tmp), 7);
    ASSERT_EQ(tmp.size(), 7);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "zxcvbnm", 7), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPFramedDataTest, ReceptionTest_withPrefixAndSuffix_4) {
    unsigned char buffer[64];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, 1, (const unsigned char *) "\x02");
    DataFrame lengthBytes(DataFrame::FRAME_TYPE_CONTENT_LENGTH, 1);
    lengthBytes.setPostExecuteFunction((const void *) &setupLengthByCommand2, (void *) &client);
    DataFrame canBytes(DataFrame::FRAME_TYPE_DATA, 1);
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, 1);
    DataFrame idBytes(DataFrame::FRAME_TYPE_DATA, 1);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame crcBytes(DataFrame::FRAME_TYPE_VALIDATOR, 2);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, 1, (const unsigned char *) "\x03");
    client = startBytes + lengthBytes + canBytes + cmdBytes + idBytes + dataBytes + crcBytes + stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:1]:<<02>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_CONTENT_LENGTH[size:1]:<<>><<exeFunc:0>><<postFunc:" + std::to_string((unsigned long) &setupLengthByCommand2) + ">>\n"
              "FRAME_TYPE_DATA[size:1]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:1]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:1]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_VALIDATOR[size:2]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:1]:<<03>><<exeFunc:0>><<postFunc:0>>\n");
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData((const unsigned char *) "\x02\x05\x13\x14\x02\x05\x13\x14\x02\x05\x13\x14\x02\x05\x13\x14\x02\x05\x13\x14\x02\x05\x13\x14\x02\x05\x13\x14\x03\x02\x05\x13\x14\x02\x05\x13\x14\x00\x12\x12\x13\x14\x03", 43), 0);
    for (int i = 0; i < 8; i++){
        ASSERT_EQ(client.receiveFramedData(), 2);
        ASSERT_EQ(client.getDataSize(), 1);
        ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 1);
        ASSERT_EQ(buffer[0], 0x02);
        ASSERT_EQ(client.getBuffer(tmp), 1);
        ASSERT_EQ(tmp.size(), 1);
        ASSERT_EQ(tmp[0], 0x02);
    }
    ASSERT_EQ(client.receiveFramedData(), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 75, true);
    ASSERT_EQ(client.getDataSize(), 10);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 10);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "\x02\x05\x13\x14\x00\x12\x12\x13\x14\x03", 10), 0);
    ASSERT_EQ(client.getBuffer(tmp), 10);
    ASSERT_EQ(tmp.size(), 10);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "\x02\x05\x13\x14\x00\x12\x12\x13\x14\x03", 10), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPFramedDataTest, ReceptionTest_withUnknownDataSz_1) {
    unsigned char buffer[64];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    client = startBytes + dataBytes + stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:4]:<<31323334>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:4]:<<39302D3D>><<exeFunc:0>><<postFunc:0>>\n");
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData("m1234567890-=qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm"), 0);
    ASSERT_EQ(client.receiveFramedData(), 0);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 75, true);
    ASSERT_EQ(client.getDataSize(), 12);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 12);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getBuffer(tmp), 12);
    ASSERT_EQ(tmp.size(), 12);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "1234567890-=", 12), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 60);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 60);
    ASSERT_EQ(memcmp(buffer, (unsigned char *) "qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm", 60), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 60);
    ASSERT_EQ(tmp.size(), 60);
    ASSERT_EQ(memcmp(tmp.data(), (unsigned char *) "qwertyuiopplkjhgfdsaZxcvbh76redcvbnm,mvdswertyuioiuhgfcxvbnm", 60), 0);
}

TEST_F(TCPFramedDataTest, ReceptionTest_withUnknownDataSz_2) {
    unsigned char buffer[16];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    int i = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    client = startBytes + dataBytes + stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:4]:<<31323334>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:4]:<<39302D3D>><<exeFunc:0>><<postFunc:0>>\n");
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData("qwertyuiop[1234567890-=zxcvbnmqwertyuiop[1234567890-=zxcvbnmqwertyuiop[1234567890-=zxcvbnmqwertyuiop[1234567890-=zxcvbnm"), 0);
    for (i = 0; i < 4; i++){
        gettimeofday(&tvStart, NULL);
        ASSERT_EQ(client.receiveFramedData(), 0);
        gettimeofday(&tvEnd, NULL);
        diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
        ASSERT_EQ(diffTime <= 75, true);
        ASSERT_EQ(client.getDataSize(), 12);
        ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 12);
        ASSERT_EQ(memcmp(buffer, (const unsigned char *) "1234567890-=", 12), 0);
        ASSERT_EQ(client.getBuffer(tmp), 12);
        ASSERT_EQ(tmp.size(), 12);
        ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "1234567890-=", 12), 0);
    }
    ASSERT_EQ(client.getRemainingDataSize(), 7);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 7);
    ASSERT_EQ(memcmp(buffer, (unsigned char *) "zxcvbnm", 7), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 7);
    ASSERT_EQ(tmp.size(), 7);
    ASSERT_EQ(memcmp(tmp.data(), (unsigned char *) "zxcvbnm", 7), 0);
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.receiveFramedData(), 2);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 240 && diffTime <= 280, true);
    ASSERT_EQ(client.getDataSize(), 7);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 7);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "zxcvbnm", 7), 0);
    ASSERT_EQ(client.getBuffer(tmp), 7);
    ASSERT_EQ(tmp.size(), 7);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "zxcvbnm", 7), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 0);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 0);
    ASSERT_EQ(tmp.size(), 0);
}

TEST_F(TCPFramedDataTest, ReadNegativeTest_1) {
    unsigned char buffer[16];
    struct timeval tvStart, tvEnd;
    int diffTime = 0;
    std::vector <unsigned char> tmp;
    client.setPort(4431);
    client.setTimeout(250);
    client.setKeepAliveMs(1000);
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    client = startBytes + cmdBytes + dataBytes + stopBytes;
    ASSERT_EQ(client.getFormat()->getDataFrameFormat(),
              "FRAME_TYPE_START_BYTES[size:4]:<<31323334>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_COMMAND[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_DATA[size:0]:<<>><<exeFunc:0>><<postFunc:0>>\n"
              "FRAME_TYPE_STOP_BYTES[size:4]:<<39302D3D>><<exeFunc:0>><<postFunc:0>>\n");
    gettimeofday(&tvStart, NULL);
    ASSERT_EQ(client.init(), 0);
    ASSERT_EQ(client.sendData("1234567890-="), 0);
    ASSERT_EQ(client.receiveFramedData(), 4);
    gettimeofday(&tvEnd, NULL);
    diffTime = (tvEnd.tv_sec - tvStart.tv_sec) * 1000 + (tvEnd.tv_usec - tvStart.tv_usec) / 1000;
    ASSERT_EQ(diffTime >= 0 && diffTime <= 75, true);
    ASSERT_EQ(client.getDataSize(), 4);
    ASSERT_EQ(client.getBuffer(buffer, sizeof(buffer)), 4);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "1234", 4), 0);
    ASSERT_EQ(client.getBuffer(tmp), 4);
    ASSERT_EQ(tmp.size(), 4);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "1234", 4), 0);
    ASSERT_EQ(client.getRemainingDataSize(), 8);
    ASSERT_EQ(client.getRemainingBuffer(buffer, sizeof(buffer)), 8);
    ASSERT_EQ(memcmp(buffer, (const unsigned char *) "567890-=", 8), 0);
    ASSERT_EQ(client.getRemainingBuffer(tmp), 8);
    ASSERT_EQ(tmp.size(), 8);
    ASSERT_EQ(memcmp(tmp.data(), (const unsigned char *) "567890-=", 8), 0);
}