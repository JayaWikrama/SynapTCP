/*
 * $Id: tcp-client.hpp,v 1.0.1 2024/09/11 12:51:47 Jaya Wikrama Exp $
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
 * @brief Enhanced TCP/IP (client) Communication Functions library.
 *
 * This file contains a collection of functions and commands designed to facilitate and extend
 * basic TCP/IP communication in C++ for the client side. These functions are intended to simplify
 * the process of setting up, sending, receiving, and managing data over TCP/IP connections.
 * The enhancements provided in this header file go beyond the standard library functions, offering
 * more flexibility and control for developers working with TCP/IP interfaces.
 *
 * The key functionalities include:
 * - Initialization and configuration of TCP/IP Client.
 * - Sending and receiving data over TCP/IP connections.
 * - Error handling and diagnostics for TCP/IP communication on client side.
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

#ifndef __TCP_CLIENT_BASIC_HPP__
#define __TCP_CLIENT_BASIC_HPP__

#include "synapsock.hpp"

#ifdef __STCP_SSL__
#include "layer-ssl.hpp"
#endif

class TCPClient : public SynapSock {
  private:
    unsigned char status;             /*!< Socket status */
#ifdef __STCP_SSL__
    SSLWarper *sslWarper;             /*!< framework to establish TLS/SSL enabled connections (this variable only available if SSL layer mode is activated) */
#endif

  protected:
    typedef enum _CLIENT_STATUS_t {   /*!< List of Socket (client) status */
      CLIENT_UNINITIALIZED = 0,       /*!< Status for TCP/IP clients that have not been initialized or when the socket has been closed and is ready to be reinitialized.*/
      CLIENT_INITIALIZED,             /*!< Status for TCP/IP client that has been initialized and ready to connect to the server.*/
      CLIENT_CONNECTED,               /*!< Status for TCP/IP client that has been initialized and ready to receive or send data to the server.*/
      CLIENT_DISCONNECTED             /*!< Status for the TCP/IP client when communication to the server has been lost.*/
    } CLIENT_STATUS_t;

    /**
     * @brief Connect to TCP/IP Server with/without timeout.
     *
     * This method is called automatically when the `clientInit` method is called.
     *
     * @return `true` if success
     * @return `false` if fail or timeout
     */
    bool connectToServer();

  public:

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
    TCPClient();

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
    TCPClient(const unsigned char *address);

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
    TCPClient(const unsigned char *address, int port);

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
    TCPClient(const std::vector <unsigned char> address);

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
    TCPClient(const std::vector <unsigned char> address, int port);

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
    TCPClient(const char *address);

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
    TCPClient(const char *address, int port);

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
    TCPClient(const std::string address);

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
    TCPClient(const std::string address, int port);

    /**
     * @brief Destructor.
     *
     * This destructor is responsible for releasing any memory that has been allocated during the object's lifetime.
     * It ensures that all allocated resources are properly freed, preventing memory leaks.
     */
    ~TCPClient();

    /**
     * @brief Initialize TCP/IP Client connection.
     *
     * This function attempts to open the specified TCP/IP (socket) port for communication and connect it to the remote server.
     * It initialize the port according to the current settings and prepares it for data transfer.
     *
     * @return `0` Initialize process success.
     * @return `1` if the port fails to open.
     * @return `2` if failed to allocate connection parameters memory
     * @return `3` if failed to get hostname ip address
     * @return `4` if failed to create SSL_CTX (available if SSL layer mode is activated)
     * @return `5` if failed to use certificate (available if SSL layer mode is activated)
     * @return `6` if failed to use private key (available if SSL layer mode is activated)
     * @return `7` if failed to check priate key (available if SSL layer mode is activated)
     * @return `8` if failed to connect server
     * @return `9` if failed to connect server with SSL handshake (available if SSL layer mode is activated)
     */
    int init();

#ifdef __STCP_SSL__
    /**
     * @brief Initialize SSL Warper.
     *
     * Initialize warper for SSL Layer protocol.
     *
     * @return `true` when success.
     * @return `false` when failed (if the SSL preprocessor is not enabled or SSL handshake is not enabled)
     */
    bool initializeSSL();
#endif

    TCPClient& operator=(const DataFrame &obj);

    TCPClient& operator+=(const DataFrame &obj);

    TCPClient& operator+(const DataFrame &obj);
};

#endif