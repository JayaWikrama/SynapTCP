#include <iostream>
#include <string.h>
#include <unistd.h>
#include "tcp-server.hpp"

const std::string errorMessage[] = {
    "Socket Port Has Not Been Initialized",
    "Timeout",
    "Frame Format Has Not Been Setup"
};

extern void displayData(const std::vector <unsigned char> &data);
extern void socketSetupDataFrameProtocol(SynapSock &obj);

void receptionCallbackFunction(SynapSock &connection, void *param){
    static int ret = 0;
    std::vector <unsigned char> data;
    std::cout << "Receiving data form " << connection.getAddress() << ":" << connection.getPort() << std::endl;
    if (connection.getFormat() == nullptr) socketSetupDataFrameProtocol(connection);
    ret = connection.receiveFramedData();
    if (ret == 0){
        std::cout << "Success to receive data: ";
        displayData(connection.getBufferAsVector());
        connection.sendData(connection.getBufferAsVector());
    }
    else {
        if (ret == 4){
            std::cout << "Failed to receive data (invalid data). Details:" << std::endl;
            if (connection.getBuffer(data) > 0){
                std::cout << "    Received Data: ";
                displayData(data);
            }
            if (connection.getRemainingBuffer(data) > 0){
                std::cout << "    Remaining Received Data: ";
                displayData(data);
            }
            std::cout << std::endl;
        }
        else {
            std::cout << errorMessage[ret - 1] << std::endl;
        }
    }
    connection.destroyFormat();
}

int main(int argc, char **argv){
    if (argc != 2){
        std::cout << "cmd: " << argv[0] << " <port>" << std::endl;
        exit(0);
    }
    TCPServer server("0.0.0.0", atoi(argv[1]));
    server.setReceptionHandler(&receptionCallbackFunction, nullptr, true);
    TCPServer::SERVER_EVENT_t event;
    SynapSock *activeConnection = nullptr;
    if (server.init() != 0){
        std::cerr << "Failed to initialize server" << std::endl;
        exit(1);
    }
    do {
        event = server.eventCheck(125);
        if (event == TCPServer::EVENT_CONNECT_REQUEST){
            std::cout << "New client accepted" << std::endl;
        }
        else if (event == TCPServer::EVENT_CLIENT_DISCONNECTED){
            std::cout << "A client disconnected from server" << std::endl;
        }
    } while (1);
    return 0;
}
