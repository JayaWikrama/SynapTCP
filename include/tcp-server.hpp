/*
 * $Id: tcp-server.hpp,v 1.0.1 2024/09/11 23:51:47 Jaya Wikrama Exp $
 *
 * Copyright (c) 2024 Jaya Wikrama
 * jayawikrama89@gmail.com
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

/**
 * @file
 * @brief Enhanced TCP/IP (server) Communication Functions library.
 *
 * This file contains a collection of functions and commands designed to facilitate and extend
 * basic TCP/IP communication in C++ for the server side. These functions are intended to simplify
 * the process of setting up, sending, receiving, and managing data over TCP/IP connections.
 * The enhancements provided in this header file go beyond the standard library functions, offering
 * more flexibility and control for developers working with TCP/IP interfaces.
 *
 * The key functionalities include:
 * - Initialization and configuration of TCP/IP Server.
 * - Sending and receiving data over TCP/IP connections.
 * - Error handling and diagnostics for TCP/IP communication on server side.
 * - Utility functions for managing TCP/IP buffers and flow control.
 *
 * The functions in this file are designed to be easy to integrate into various projects,
 * providing a robust foundation for TCP/IP communication in embedded systems, networking,
 * or any application that requires TCP/IP data transmission.
 *
 * @note This file is a part of a larger project focusing on enhancing TCP/IP communication
 *       capabilities in C++ applications.
 *
 * @version 1.0.0
 * @date 2024-08-31
 * @author Jaya Wikrama
 *
 * @version 1.0.1
 * @date 2024-09-11
 * @author Jaya Wikrama
 * @note Splitting tcp.h into socket.hpp tcp-client.hpp tcp-server.hpp
 */

#ifndef __TCP_SERVER_BASIC_HPP__
#define __TCP_SERVER_BASIC_HPP__

#include "synapsock.hpp"

#ifdef __STCP_SSL__
#include "layer-ssl.hpp"
#endif

class ClientCollection {
  public:
    SynapSock *client;
    struct timeval lastActivity;
    pthread_t th;
    pthread_cond_t cond;
    pthread_mutex_t mtx;
    int pipe[2];
    ClientCollection *next;

    ClientCollection(const SynapSock *client);
    ~ClientCollection();
};

class TCPServer : public SynapSock {
  private:
    bool receptionHandlerAsThread;          /*!< mode to choose how the reception handler is run (as a thread or not) */
    unsigned short maxClient;               /*!< maximum number of client (for server) */
    SynapSock *client;                      /*!< client pointer that is being processed */
    ClientCollection *clientList;           /*!< a collection of TCP/IP clients that have been accepted by the server */
    const void *conReqCallbackFunction;     /*!< callback function that is automatically called when there is a connection request event */
    void *conReqCallbackParam;              /*!< parameters of the connection request event callback function */
    const void *receptionCallbackFunction;  /*!< callback function that is automatically called when there is a reception event */
    void *receptionCallbackParam;           /*!< parameters of the reception event callback function */
#ifdef __STCP_SSL__
    SSLWarper *sslWarper;                   /*!< framework to establish TLS/SSL enabled connections (this variable only available if SSL layer mode is activated) */
#endif

    /* Set some parent method as private method on TCPServer */
    using SynapSock::getFormat;
    using SynapSock::destroyFormat;
    using SynapSock::trigInvDataIndicator;
    using SynapSock::receiveFramedData;
    using SynapSock::sendFramedData;
    using SynapSock::getSpecificBufferAsVector;
    using Socket::duplicate;
    using Socket::isInputBytesAvailable;
    using Socket::receiveData;
    using Socket::receiveStartBytes;
    using Socket::receiveUntillStopBytes;
    using Socket::receiveStopBytes;
    using Socket::receiveNBytes;
    using Socket::getDataSize;
    using Socket::getBuffer;
    using Socket::getBufferAsVector;
    using Socket::getRemainingDataSize;
    using Socket::getRemainingBuffer;
    using Socket::getRemainingBufferAsVector;
    using Socket::sendData;
    using Socket::closeConnection;

  protected:
    /**
     * @brief Add new client that has been accepted/connected by server to the client list.
     *
     * This method is called automatically when the `acceptNewClient` method is called sucessfully.
     * @param[in] connFd File descriptor for accepted client connection.
     * @return `true` on success
     * @return `false` if failed
     */
    bool addClient(int connFd);

    /**
     * @brief Remove existing client that has been disconnected by server from the client list.
     *
     * This method is called automatically when the `eventCheck` method is called sucessfully and disconnected event available.
     * @return `true` on success
     * @return `false` if failed
     */
    bool removeClient(const SynapSock *socket);

  public:
    typedef enum _SERVER_EVENT_t {        /*!< Available event on server side after server has been initialized. To check current available event, you can call `eventCheck` method */
      EVENT_NONE = 0,                     /*!< when nothing happens */
      EVENT_CONNECT_REQUEST = 1,          /*!< when there is a connection request from the client side (when receiving this event, the server side needs to call the `acceptNewClient` method) */
      EVENT_BYTES_AVAILABLE = 2,          /*!< when there is data sent from the client side (to obtain this data, the server needs to call the reception method) */
      EVENT_CLIENT_DISCONNECTED = 3       /*!< when a client disconnects from the server */
    } SERVER_EVENT_t;

    /**
     * @brief Default constructor.
     *
     * This constructor initializes private and protected data members and parameters to their default values, including:
     * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `address = 127.0.0.1` : Default address is set to localhost.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     */
    TCPServer();
    /**
     * @brief Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     */
    TCPServer(const unsigned char *address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     * @param[in] port The port of TCP/IP interface.
     */
    TCPServer(const unsigned char *address, int port);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     */
    TCPServer(const std::vector <unsigned char> address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     * @param[in] port The port of TCP/IP interface.
     */
    TCPServer(const std::vector <unsigned char> address, int port);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (in this case, a string in the form of a char pointer).
     */
    TCPServer(const char *address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (in this case, a string in the form of a char pointer).
     * @param[in] port The port of TCP/IP interface.
     */
    TCPServer(const char *address, int port);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (string).
     */
    TCPServer(const std::string address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `maxClient = 10` : Default value of maximum number of client (for server) is 10.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (string).
     * @param[in] port The port of TCP/IP interface.
     */
    TCPServer(const std::string address, int port);

    /**
     * @brief Destructor.
     *
     * This destructor is responsible for releasing any memory that has been allocated during the object's lifetime.
     * It ensures that all allocated resources are properly freed, preventing memory leaks.
     */
    ~TCPServer();

    /**
     * @brief Set the maximum clients that the TCP/IP Server can handle.
     *
     * This method is responsible for setting the maximum number of clients that the TCP/IP Server can handle. This method only for server side.
     *
     * @param[in] nClient the maximum number of clients.
     * @return `true` when the client number is valid
     * @return `false` when the client number is invalid
     */
    bool setMaximumClient(int nClient);

    /**
     * @brief Gets the maximum clients that the TCP/IP Server can handle.
     *
     * This getter function retrieves the maximum clients (the TCP/IP Server can handle) that currently configured for TCP/IP communication interface.
     *
     * @return The number of client that the TCP/IP Server can handle.
     */
    int getMaximumClient();

    /**
     * @brief Initialize TCP/IP Server connection.
     *
     * This function attempts to open the specified TCP/IP (socket) port for communication. It initialize the port according to the current settings and prepares it for data transfer.
     *
     * @return `0` Initialize process success.
     * @return `1` if the port fails to open.
     * @return `2` if failed to allocate connection parameters memory.
     * @return `3` if failed to bind connection
     * @return `4` if failed to listen connection
     * @return `5` if failed to create SSL_CTX (available if SSL layer mode is activated)
     * @return `6` if failed to use certificate (available if SSL layer mode is activated)
     * @return `7` if failed to use private key (available if SSL layer mode is activated)
     * @return `8` if failed to check priate key (available if SSL layer mode is activated)
     * @return `9` if failed to load verify location (available if SSL layer mode is activated)
     * @return `10` if failed to load cacert (available if SSL layer mode is activated)
     */
    int init();

    /**
     * @brief Check available event on server side after server has been initialized.
     *
     * This function attempts to check available event on server side after server has been initialized.
     *
     * @param[in] timeoutMs maximum waiting time to check event.
     * @return `EVENT_NONE` when nothing happens
     * @return `EVENT_CONNECT_REQUEST` when there is a connection request from the client side (when receiving this event, the server side needs to call the `acceptNewClient` method)
     * @return `EVENT_BYTES_AVAILABLE` when there is data sent from the client side (to obtain this data, the server needs to call the reception method)
     * @return `EVENT_CLIENT_DISCONNECTED` when a client disconnects from the server
     */
    TCPServer::SERVER_EVENT_t eventCheck(unsigned short timeoutMs);

    /**
     * @brief Accept the available client when TCP/IP Server listen the connection.
     *
     * This function attempts to accept the connection that requested by client side.
     *
     * @return `true` in success.
     * @return `false` if failed.
     */
    bool acceptNewClient();

    /**
     * @brief Gets the pointer of the currently active client connection.
     *
     * This method is responsible for returning the pointer value of an active client connection.
     *
     * @return pointer of active client object.
     * @return `nullptr` if no active client available.
     */
    SynapSock *getActiveClient();

    /**
     * @brief Provides access to users to manage connection requests.
     *
     * This method provide flexibility to users to manage existing connection requests.
     * The callback function is automatically called when there is a connection request event.
     * If the callback function is not set up, the connection request will be automatically
     * approved (accepted) by the server.
     *
     * @param[in] func callback function that has 2 parameters. `TCP Server &` is an object of the server itself. `void *` is a pointer that will connect directly to `void *param`.
     * @param[in] param callback function parameter.
     */
    void setConnectionRequestHandler(void (*func)(TCPServer &, void *), void *param);

    /**
     * @brief Set handler to receive data sent by remote client.
     *
     * This method must be called before socket communication begins (before calling the `eventCheck` method).
     *
     * @param[in] func callback function that has 2 parameters. `SynapSock &` is an active connection. `void *` is a pointer that will connect directly to `void *param`
     * @param[in] param callback function parameter.
     * @param[in] asThread if the given value is true, then the reception handler will run as a thread.
     */
    void setReceptionHandler(void (*func)(SynapSock &, void *), void *param, bool asThread);

    /**
     * @brief Method overloading setReceptionHandler. Set handler to receive data sent by remote client for non-thread operation.
     *
     * This method must be called before socket communication begins (before calling the `eventCheck` method).
     *
     * @param[in] func callback function that has 2 parameters. `SynapSock &` is an active connection. `void *` is a pointer that will connect directly to `void *param`
     * @param[in] param callback function parameter.
     */
    void setReceptionHandler(void (*func)(SynapSock &, void *), void *param);

    /**
     * @brief Retrieves the pointer address for the data reception handler function.
     *
     * @return pointer of reception handler function.
     */
    const void *getReceptionHandlerFunction();

    /**
     * @brief Retrieves the pointer address for the data reception handler parameter.
     *
     * @return pointer of reception handler parameter.
     */
    void *getReceptionHandlerParam();

#ifdef __STCP_SSL__
    /**
     * @brief Initialize SSL Warper.
     *
     * Initialize warper for SSL Layer protocol.
     *
     * @param[in] cert Optional string containing the server's certificate in PEM format. Only used for server.
     * @param[in] key Optional string containing the server's private key in PEM format. Only used for server.
     * @return `true` when success.
     * @return `false` when failed (if the SSL preprocessor is not enabled or SSL handshake is not enabled)
     */
    bool initializeSSL(const std::string cert, const std::string key);
#endif
};
#endif