#include <iostream>
#include <string.h>

#include "tcp-client.hpp"

const std::string errorMessage[] = {
    "Port Has Not Been Opened",
    "Timeout",
    "Frame Format Has Not Been Setup"
};

extern void displayData(const std::vector <unsigned char> &data);
extern void socketSetupDataFrameProtocol(SynapSock &obj);
extern std::vector <unsigned char> base16decode(const char *data);

int main(int argc, char **argv){
    if (argc != 6){
        std::cout << "cmd: " << argv[0] << " <address> <port> <timeoutMs> <keepAliveMs> <data>" << std::endl;
        exit(0);
    }
    int ret = 0;
    std::vector <unsigned char> data;
    TCPClient client(argv[1], atoi(argv[2]));
    /* Prepare Object */
    client.setTimeout(atoi(argv[3]));
    client.setKeepAlive(atoi(argv[4]));
    socketSetupDataFrameProtocol(client);
    DataFrame *frame = client[1];
    /* Do socket communication */
    ret = client.init();
    if (ret != 0){
        std::cerr << "Failed to initialize server [" << ret << "]" << std::endl;
        exit(1);
    }
    data = base16decode(argv[5]);
    std::cout << "Send: ";
    displayData(data);
    if (client.sendData(data) != 0){
        client.closeSocket();
        exit(2);
    }
    if ((ret = client.receiveFramedData()) != 0){
        if (ret == 4){
            std::cout << "Invalid Received Data Details:" << std::endl;
            if (client.getBuffer(data) > 0){
                std::cout << "    Received Data: ";
                displayData(data);
            }
            if (client.getRemainingBuffer(data) > 0){
                std::cout << "    Remaining Received Data: ";
                displayData(data);
            }
            std::cout << std::endl;
        }
        else {
            std::cout << errorMessage[ret - 1] << std::endl;
        }
    }
    else {
        client.getFormat()->getAllData(data);
        std::cout << "Received Success [" + std::to_string(client.getDataSize()) + "]" << std::endl;
        std::cout << "    Data: ";
        displayData(data);
    }
    client.closeSocket();
    return 0;
}
