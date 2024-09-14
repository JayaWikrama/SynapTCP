#include <iostream>
#include <string.h>
#include <unistd.h>
#include "tcp-client.hpp"
#include "tcp-server.hpp"
#include "synapsock.hpp"

void displayData(const std::vector <unsigned char> &data){
    for (auto i = data.begin(); i < data.end(); ++i){
        std::cout << std::hex << (int) *i;
        std::cout << " ";
    }
    std::cout << std::endl;
}

void crc16(DataFrame &frame, void *ptr) {
    /* Get SynapSock object form function param */
    SynapSock *tmp = (SynapSock *) ptr;
    /* Initialize crc16 param */
    unsigned short crc = 0x0000;
    unsigned short poly = 0x1021;
    /* Get data from Start Bytes until Data */
    std::vector <unsigned char> data = tmp->getSpecificBufferAsVector(DataFrame::FRAME_TYPE_START_BYTES, DataFrame::FRAME_TYPE_DATA);
    std::cout << "Data from which the CRC value will be calculated:" << std::endl;
    displayData(data);
    /* Calculate crc16 */
    for (const auto &byte : data) {
        crc ^= (static_cast<unsigned short>(byte) << 8);
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    /* Compare received CRC with calculated CRC */
    unsigned short rcvCRC = 0;
    frame.getData((unsigned char *) &rcvCRC, 2);
    if (rcvCRC != crc){
        /* invalid crc -> trigger stop flag to __readFramedData__ method */
        tmp->trigInvDataIndicator();
        std::cout << "CRC16 Invalid (0x" << std::hex << rcvCRC << " != 0x" << std::hex << crc << ")" << std::endl;
    }
}

void crc16ForClientSide(DataFrame &frame, void *ptr){

}

std::vector <unsigned char> base16decode(const char *data){
    std::vector <unsigned char> result;
    if (strlen(data) % 2 == 1) return result;
    unsigned char tmp = 0x00;
    while (*data != 0x00){
        if (*data >= '0' && *data <= '9') tmp = (*data) - '0';
        else if (*data >= 'a' && *data <= 'z') tmp = (*data) - 'a';
        else if (*data >= 'A' && *data <= 'Z') tmp = (*data) - 'A';
        data++;
        tmp *= 16;
        if (*data >= '0' && *data <= '9') tmp += ((*data) - '0');
        else if (*data >= 'a' && *data <= 'z') tmp += ((*data) - 'a') + 10;
        else if (*data >= 'A' && *data <= 'Z') tmp += ((*data) - 'A') + 10;
        data++;
        result.push_back(tmp);
    }
    return result;
}

void setupLengthByCommand(DataFrame &frame, void *ptr){
    int data = 0;
    /* Get SynapSock object form function param */
    SynapSock *tmp = (SynapSock *) ptr;
    /* Get DataFrame target */
    DataFrame *target = (*tmp)[DataFrame::FRAME_TYPE_DATA];
    if (target == nullptr) return;
    frame.getData((unsigned char *) &data, 1);
    if (data == 0x35){
        /* setup 3 as data size of DataFrame::FRAME_TYPE_DATA */
        target->setSize(3);
    }
    else if (data == 0x36){
        /* setup 3 as data size of DataFrame::FRAME_TYPE_DATA */
        target->setSize(2);
    }
    else {
        /* invalid value found -> trigger stop flag to __readFramedData__ method */
        tmp->trigInvDataIndicator();
    }
}

void socketSetupDataFrameProtocol(SynapSock &obj){
    /* Frame Data Format / Protocol Example
     * | Start  Bytes |   Command   |  Main Data   | CRC16 Validator | Stop Bytes |
     * |:-------------|:------------|:-------------|:----------------|:-----------|
     * |    4 bytes   |   1 byte    |   N bytes    |     2 bytes     | 4 bytes    |
     * |  0x31323334  | 0x35 / 0x36 | based on Cmd |  init = 0x0000  | 0x39302D3D |
     *
     * If Command = 0x35, then the Main Data length is 3 bytes.
     * If Command = 0x36, then the Main Data length is 2 bytes.
     * If Command is not equal to 0x35 and 0x36, then the data is invalid.
     *
     * example: 3132333435363738159039302D3D
     */
    /* Configure Frame Format */
    DataFrame startBytes(DataFrame::FRAME_TYPE_START_BYTES, "1234");
    DataFrame cmdBytes(DataFrame::FRAME_TYPE_COMMAND, 1);
    /* Setup the handler function to determine the data length of DataFrame::FRAME_TYPE_DATA.
     * This function is called after all data from DataFrame::FRAME_TYPE_COMMAND is received.
     */
    cmdBytes.setPostExecuteFunction((const void *) &setupLengthByCommand, (void *) &obj);
    DataFrame dataBytes(DataFrame::FRAME_TYPE_DATA);
    DataFrame crcValidatorBytes(DataFrame::FRAME_TYPE_VALIDATOR, 2);
    /* Setup the handler function to validate Data by using crc16 validation.
     * This function is called after all data from DataFrame::FRAME_TYPE_VALIDATOR is received.
     */
    crcValidatorBytes.setPostExecuteFunction((const void *) &crc16, (void *) &obj);
    DataFrame stopBytes(DataFrame::FRAME_TYPE_STOP_BYTES, "90-=");
    /* Setup Frame Format to SynapSock com */
    obj = startBytes + cmdBytes + dataBytes + crcValidatorBytes + stopBytes;
}