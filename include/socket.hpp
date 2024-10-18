/*
 * $Id: socket.hpp,v 1.0.0 2024/09/10 22:31:12 Jaya Wikrama Exp $
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
 * @brief Enhanced Socket Communication Functions library.
 *
 * This file contains a collection of functions and commands designed to facilitate and extend
 * basic Socket communication in C++. These functions are intended to simplify the process of
 * setting up, sending, receiving, and managing data over Socket connections. The enhancements
 * provided in this header file go beyond the standard library functions, offering more
 * flexibility and control for developers working with Socket interfaces.
 *
 *
 * The functions in this file are designed to be easy to integrate into various projects,
 * providing a robust foundation for socket communication in embedded systems, networking,
 * or any application that requires socket data transmission.
 *
 * @note This file is a part of a larger project focusing on enhancing Socket or UDP communication
 *       capabilities in C++ applications.
 *
 * @version 1.0.0
 * @date 2024-09-10
 * @author Jaya Wikrama
 */

#ifndef __SOCKET_BASIC_HPP__
#define __SOCKET_BASIC_HPP__

#include <string>
#include <vector>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#ifdef __STCP_SSL__
#include "layer-ssl.hpp"
#endif

class Socket {
  protected:
    std::vector <unsigned char> address;  /*!< socket address */
    struct sockaddr_in addr;              /*!< structures for handling internet addresses */
    int sockFd;                           /*!< socket file descriptor */
    unsigned short keepAliveMs;           /*!< maximum wait time for receiving the next byte of data after the initial byte has been received */
    struct timeval tvTimeout;             /*!< store timeout connection parameters */
    #ifdef __STCP_SSL__
        bool useSSL;                      /*!< variable to activate SSL Mode Protection */
        bool sslVerifyMode;               /*!< mode of ssl connection routine, false to disable certificate verification process and true value to enable certificate verification process (this variable only available if SSL layer mode is activated) */
        int sslConnRoutineOkStatus;       /*!< variable to store the reason of disconnected event, 0 for normal routine and -1 for specific case (this variable only available if SSL layer mode is activated) */
        SSL *sslConn;                     /*!< SSL file descriptor (this variable only available if SSL layer mode is activated) */
    #endif
    pthread_mutex_t mtx;                  /*!< locking mechanism for common method */
    pthread_mutex_t wmtx;                 /*!< locking mechanism for write method */
    std::vector <unsigned char> data;           /*!< variable that store received data */
    std::vector <unsigned char> remainingData;  /*!< variable that store remaining data */

  public:
    /**
     * @brief Default constructor.
     *
     * This constructor initializes private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `address = 127.0.0.1` : Default address is set to localhost.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     */
    Socket();

    /**
     * @brief Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     */
    Socket(const unsigned char *address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     * @param[in] port The port of Socket interface.
     */
    Socket(const unsigned char *address, int port);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     */
    Socket(const std::vector <unsigned char> address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address is in the form of an IP address with a size of 4 bytes.
     * @param[in] port The port of Socket interface.
     */
    Socket(const std::vector <unsigned char> address, int port);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (in this case, a string in the form of a char pointer).
     */
    Socket(const char *address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (in this case, a string in the form of a char pointer).
     * @param[in] port The port of Socket interface.
     */
    Socket(const char *address, int port);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `port = 3000` : Port is set to 3000 as its default value.
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (string).
     */
    Socket(const std::string address);

    /**
     * @brief Overloading of Custom constructor.
     *
     * This constructor initializes some private and protected data members and parameters to their default values, including:
     * - `timeout = 1500` : Timeout is set to 1500 ms (1.5 second).
     * - `keepAliveMs = 0` : Keep-alive interval is set to 0 milliseconds.
     * - Initializes the mutex for thread safety.
     * @param[in] address The address in the form of an IP address or domain (string).
     * @param[in] port The port of Socket interface.
     */
    Socket(const std::string address, int port);

    /**
     * @brief Destructor.
     *
     * This destructor is responsible for releasing any memory that has been allocated during the object's lifetime.
     * It ensures that all allocated resources are properly freed, preventing memory leaks.
     */
    ~Socket();

    /**
     * @brief Check is IP Address valid or not.
     *
     * This method is responsible for validating whether the IP Address is valid or not.
     *
     * @param[in] address The IP Address (with a size of 4 bytes).
     * @return `true` when the IP Address is valid
     * @return `false` when the IP Address is invalid
     */
    bool isValidIPAddress(const unsigned char *address);

    /**
     * @brief Overloaded method for `isValidIPAddress` to check is IP Address valid or not.
     *
     * This method is responsible for validating whether the IP Address is valid or not.
     *
     * @param[in] address The IP Address (with a size of 4 bytes).
     * @return `true` when the IP Address is valid
     * @return `false` when the IP Address is invalid
     */
    bool isValidIPAddress(const std::vector <unsigned char> address);

    /**
     * @brief Overloaded method for `isValidIPAddress` to check is IP Address valid or not.
     *
     * This method is responsible for validating whether the IP Address is valid or not.
     *
     * @param[in] address The IP Address in string form.
     * @return `true` when the IP Address is valid
     * @return `false` when the IP Address is invalid
     */
    bool isValidIPAddress(const char *address);

    /**
     * @brief Overloaded method for `isValidIPAddress` to check is IP Address valid or not.
     *
     * This method is responsible for validating whether the IP Address is valid or not.
     *
     * @param[in] address The IP Address in string form.
     * @return `true` when the IP Address is valid
     * @return `false` when the IP Address is invalid
     */
    bool isValidIPAddress(const std::string address);

    /**
     * @brief Set Address of Socket Interface.
     *
     * This method is responsible for setting Address in IP Address form (with a size of 4 bytes).
     *
     * @param[in] address The IP Address (with a size of 4 bytes).
     * @return `true` when the IP Address is valid
     * @return `false` when the IP Address is invalid
     */
    bool setAddress(const unsigned char *address);

    /**
     * @brief Overloaded method for `setAddress` to set Address of Socket Interface.
     *
     * This method is responsible for setting Address in IP Address form (with a size of 4 bytes).
     *
     * @param[in] address The IP Address (with a size of 4 bytes).
     * @return `true` when the IP Address is valid
     * @return `false` when the IP Address is invalid
     */
    bool setAddress(const std::vector <unsigned char> address);

    /**
     * @brief Overloaded method for `setAddress` to set Address of Socket Interface.
     *
     * This method is responsible for setting Address in IP Address or domain in string form.
     *
     * @param[in] address The IP Address (in string form).
     * @return `true` when the Address is valid (base on its pattern)
     * @return `false` when the Address is invalid
     */
    bool setAddress(const std::string address);

    /**
     * @brief Overloaded method for `setAddress` to set Address of Socket Interface.
     *
     * This method is responsible for setting Address in IP Address or domain in string form.
     *
     * @param[in] address The IP Address (in string form).
     * @return `true` when the Address is valid (base on its pattern)
     * @return `false` when the Address is invalid
     */
    bool setAddress(const char *address);

    /**
     * @brief Set Socket port interface.
     *
     * This method is responsible for setting port of Socket interface.
     *
     * @param[in] port port communication.
     * @return `true` when the port is valid
     * @return `false` when the port is invalid
     */
    bool setPort(int port);

    /**
     * @brief Sets the communication timeout in milliseconds.
     *
     * This setter function configures the timeout for Socket communication. The timeout value is specified in units of 1 milliseconds.
     *
     * @param[in] milliseconds The timeout value.
     * @return `true` if the input value is valid.
     * @return `false` if the input value is invalid.
     */
    bool setTimeout(int milliseconds);

    /**
     * @brief Overloaded method in `setTimeout` to sets the communication timeout in seconds and milliseconds.
     *
     * This setter function configures the timeout for Socket communication. The timeout value is specified in units of 1 seconds and 1 milliseconds.
     *
     * @param[in] seconds The timeout value in seconds.
     * @param[in] milliseconds The timeout value in milliseconds.
     * @return `true` if the input value is valid.
     * @return `false` if the input value is invalid.
     */
    bool setTimeout(int seconds, int milliseconds);

    /**
     * @brief Sets the keep-alive interval for communication.
     *
     * This setter function configures the maximum wait time for receiving the next byte of data after the initial byte has been received. This helps maintain the connection by ensuring timely data reception.
     *
     * @param keepAliveMs The keep-alive interval in milliseconds.
     */
    bool setKeepAliveMs(int keepAliveMs);

    /**
     * @brief Sets the SSL protection mode (is activated or not).
     *
     * This setter function configures the use of SSL handshake. This helps to improve the security of Socket communication.
     *
     * @param[in] isUseSSL The value true is assigned to enable SSL handshake.
     * @return `true` when success.
     * @return `false` when failed (if the SSL preprocessor is not enabled)
     */
    bool setIsUseSSL(bool isUseSSL);

    /**
     * @brief Sets the SSL verify mode (is activated or not).
     *
     * This setter function configures the use of SSL verify mode.
     *
     * @param[in] sslVerifyMode The value true is assigned to enable SSL verify mode.
     * @return `true` when success.
     * @return `false` when failed (if the SSL preprocessor is not enabled or SSL handshake is not enabled)
     */
    bool setSSLVerifyMode(bool sslVerifyMode);

#ifdef __STCP_SSL__
    /**
     * @brief Sets the SSL pointer.
     *
     * This setter function assign the SSL pointer.
     *
     * @param[in] sslConn pointer.
     * @return `true` when success.
     * @return `false` when failed (if the SSL preprocessor is not enabled)
     */
    bool setSSLPointer(SSL *sslConn);
#endif

    /**
     * @brief Manually setup the file descriptor.
     *
     * This setter function is used to manually setup or assign file descriptors.
     *
     * @param[in] sockFd The file descriptor
     * @return `true` for valid file descriptor
     * @return `false` for invalid file descriptor
     */
    bool setSocketFd(int sockFd);

    /**
     * @brief Gets the address of Socket communication interface.
     *
     * This getter function retrieves address currently configured for Socket communication interface.
     *
     * @return The address of Socket Interface.
     */
    std::string getAddress();

    /**
     * @brief Gets the port for Socket communication interface.
     *
     * This getter function retrieves port that currently configured for Socket communication interface.
     *
     * @return The port for Socket communication interface.
     */
    int getPort();

    /**
     * @brief Gets the communication timeout of Socket communication.
     *
     * This getter function retrieves the communication timeout that currently configured for Socket communication.
     *
     * @return The communication timeout.
     */
    const struct timeval *getTimeout();

    /**
     * @brief Gets the keep-alive interval for communication.
     *
     * This getter function retrieves the maximum wait time configured for receiving the next byte of data after the initial byte has been successfully received. The interval is specified in milliseconds.
     *
     * @return The keep-alive interval in milliseconds.
     */
    int getKeepAliveMs();

    /**
     * @brief Gets is the SSL handshake active or not.
     *
     * This getter function retrieves is the SSL handshake active or not for Socket communication.
     *
     * @return SSL handshake (use) status.
     */
    bool getIsUseSSL();

    /**
     * @brief Gets is the SSL verify mode active or not.
     *
     * This getter function retrieves is the SSL verify mode active or not for Socket communication.
     *
     * @return SSL verify mode status.
     */
    bool getSSLVerifyMode();

    /**
     * @brief Gets the file descriptor.
     *
     * This getter function is used to get the file descriptors.
     *
     * @return file descriptor.
     */
    int getSocketFd();

    /**
     * @brief Checks whether socket communication has timed out based on its last activity.
     *
     * This function works by calculating the difference between the reference time and the socket's last activity time.
     * Then this value is compared with the timeout limit value of the socket.
     *
     * @param[in] ref The reference time.
     * @param[in] lastActivity The socket's last activity time.
     * @return `true` if timeout.
     * @return `false` if it hasn't timed out yet.
     */
    bool isSocketTimeout(const struct timeval *ref, const struct timeval *lastActivity);

    /**
     * @brief duplicate socket and its parameters.
     *
     * This function is responsible for duplicating all the parameters of its parent object except for SSL pointers.
     * Both objects use the same SSL pointer. So before deleting one of the objects, make sure to assign the SSL pointer
     * from one of the objects to the NULL value.
     *
     * @param[in] obj The target object.
     * @return `true` in success.
     * @return `false` if failed.
     */
    bool duplicate(Socket &obj);

    /**
     * @brief Checks for available input bytes.
     *
     * This function checks whether there are any bytes available in the socket buffer.
     *
     * @return `true` if there are bytes available in the socket buffer.
     * @return `false` if there are no bytes available in the socket buffer.
     */
    bool isInputBytesAvailable();
    /**
     * @brief Performs a Socket data receive operation.
     *
     * This function receives data from the Socket port without separating the successfully received data into the desired size and remaining data. The receive Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] sz The number of bytes to receive. A value of `0` means that the receiving operation is unlimited (up to the `keepAliveMs` timeout).
     * @param[in] dontSplitRemainingData A flag to disable automatic data splitting based on the amount of data requested.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveData(size_t sz, bool dontSplitRemainingData);

    /**
     * @brief Overloaded method for `receiveData` to perform Socket data reception.
     *
     * This overloaded method performs a Socket data receiving operation.
     *
     * @param[in] sz The number of bytes to receive. A value of `0` means that the receiving operation is unlimited (up to the `keepAliveMs` timeout).
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveData(size_t sz);

    /**
     * @brief Overloaded method for `receiveData` to perform Socket data reception.
     *
     * This overloaded method performs a Socket data receiving operation.
     *
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveData();

    /**
     * @brief Recieves Socket data until the desired start bytes are found.
     *
     * This function performs a Socket data receiving operation until the specified start bytes are detected. Any Socket data received before the start bytes are found is automatically discarded. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] startBytes A pointer to the start bytes data to be detected.
     * @param[in] sz The size of the start bytes data to be detected.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveStartBytes(const unsigned char *startBytes, size_t sz);

    /**
     * @brief Overloaded method for `receiveStartBytes` with `std::vector` input.
     *
     * This overloaded function performs a Socket data reception operation until the specified start bytes are detected. Any Socket data received before the start bytes are found is automatically discarded. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] startBytes A vector containing the start bytes data to be detected.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveStartBytes(const std::vector <unsigned char> startBytes);

    /**
     * @brief Overloaded method for `receiveStartBytes` with `const char*` input.
     *
     * This overloaded function performs a Socket data reception operation until the specified start bytes are detected. Any Socket data received before the start bytes are found is automatically discarded. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] startBytes A pointer to the start bytes data to be detected, provided as a `const char*`.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveStartBytes(const char *startBytes);

    /**
     * @brief Overloaded method for `receiveStartBytes` with `std::string` input.
     *
     * This overloaded function performs a Socket data reception operation until the specified start bytes are detected. Any Socket data received before the start bytes are found is automatically discarded. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] startBytes A string containing the start bytes data to be detected.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveStartBytes(const std::string startBytes);

    /**
     * @brief Performs a Socket data receiving operation until the specified stop bytes are detected.
     *
     * This function receives Socket data until the specified stop bytes are detected. Any Socket data received up to and including the stop bytes is automatically stored in the buffer. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] stopBytes The data representing the stop bytes to be detected.
     * @param[in] sz The size of the stop bytes data to be detected.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveUntillStopBytes(const unsigned char *stopBytes, size_t sz);

    /**
     * @brief Overloaded function for `receiveUntillStopBytes` with input as `std::vector`.
     *
     * This function receives Socket data until the specified stop bytes are detected. Any Socket data received up to and including the stop bytes is automatically stored in the buffer. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] stopBytes A vector of `unsigned char` representing the stop bytes to be detected.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveUntillStopBytes(const std::vector <unsigned char> stopBytes);
    
    /**
     * @brief Overloaded function for `receiveUntillStopBytes` with input as `const char*`.
     *
     * This function receives Socket data until the specified stop bytes are detected. Any Socket data received up to and including the stop bytes is automatically stored in the buffer. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] stopBytes A pointer to a null-terminated character array representing the stop bytes to be detected.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveUntillStopBytes(const char *stopBytes);

    /**
     * @brief Overloaded function for `receiveUntillStopBytes` with input as `std::string`.
     *
     * This function receives Socket data until the specified stop bytes are detected. Any Socket data received up to and including the stop bytes is automatically stored in the buffer. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] stopBytes A string representing the stop bytes to be detected.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveUntillStopBytes(const std::string stopBytes);

    /**
     * @brief Receive Socket data and checks if the data matches the specified stop bytes.
     *
     * This method performs Socket data reception while simultaneously checking if the data matches the desired stop bytes. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] stopBytes A pointer to the stop bytes data to be detected.
     * @param[in] sz The size of the stop bytes data to be detected.
     * @return `0` if the operation is successful and the data is valid.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     * @return `3` if data is recieved but does not match the specified stop bytes.
     */
    int receiveStopBytes(const unsigned char *stopBytes, size_t sz);

    /**
     * @brief Function overloading for `receiveStopBytes` with input using `std::vector`.
     *
     * This method performs Socket data reception while simultaneously checking if the data matches the desired stop bytes. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] stopBytes A `std::vector` containing the stop bytes data to be detected.
     * @return `0` if the operation is successful and the data is valid.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     * @return `3` if data is received but does not match the specified stop bytes.
     */
    int receiveStopBytes(const std::vector <unsigned char> stopBytes);

    /**
     * @brief Function overloading for `receiveStopBytes` with input using `char *`.
     *
     * This method performs Socket data reception while simultaneously checking if the data matches the desired stop bytes. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] stopBytes A C-style string (null-terminated) representing the stop bytes to be detected.
     * @return `0` if the operation is successful and the data is valid.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     * @return `3` if data is received but does not match the specified stop bytes.
     */
    int receiveStopBytes(const char *stopBytes);

    /**
     * @brief Function overloading for `receiveStopBytes` with input using `std::string`.
     *
     * This method performs Socket data reception while simultaneously checking if the data matches the desired stop bytes. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] stopBytes A `std::string` object representing the stop bytes to be detected.
     * @return `0` if the operation is successful and the data is valid.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     * @return `3` if data is received but does not match the specified stop bytes.
     */
    int receiveStopBytes(const std::string stopBytes);

    /**
     * @brief Performs Socket data reception until the desired amount of data is received.
     *
     * This function performs the operation of receiving Socket data until the specified amount of data is fulfilled. The operation retries up to 3 times, starting from the first data received. The received Socket data can be accessed using the `Socket::getBuffer` method.
     *
     * @param[in] sz The size of the Socket data to be received.
     * @return `0` if successful.
     * @return `1` if the port is not open.
     * @return `2` if a timeout occurs.
     */
    int receiveNBytes(size_t sz);

    /**
     * @brief Retrieves the amount of successfully received data.
     *
     * This function retrieves the information about the size of the data that has been successfully received.
     *
     * @return The size of the data in bytes.
     */
    size_t getDataSize();

    /**
     * @brief Retrieves the received data buffer.
     *
     * This function retrieves all the data that has been successfully received by the `receive` method.
     *
     * @param[out] buffer A variable to hold the data that has been successfully received.
     * @param[in] maxBufferSz The maximum size of the data that can be accommodated in the buffer.
     * @return The size of the data received.
     */
    size_t getBuffer(unsigned char *buffer, size_t maxBufferSz);

    /**
     * @brief Overloaded method for `getBuffer` with `std::vector<unsigned char>` as the output parameter.
     *
     * Retrieves all the data that has been successfully received by the `receive` method.
     *
     * This overload allows you to use a `std::vector<unsigned char>` as the buffer to hold the received data.
     *
     * @param[out] buffer A `std::vector<unsigned char>` to hold the received data that has been successfully received.
     * @return The size of the data received.
     */
    size_t getBuffer(std::vector <unsigned char> &buffer);

    /**
     * @brief Retrieves the received data buffer as a vector.
     *
     * This method returns all the data that has been successfully received by the `receive` method as a `std::vector<unsigned char>`.
     *
     * @return A `std::vector<unsigned char>` containing the data that has been successfully recieved.
     */
    std::vector <unsigned char> getBufferAsVector();

    /**
     * @brief Retrieves the number of bytes in the remaining buffer.
     *
     * This method provides the size of the data that is still present in the remaining buffer.
     *
     * @return The size of the remaining data in bytes.
     */
    size_t getRemainingDataSize();

    /**
     * @brief Retrieves the remaining recieved data outside of the data buffer.
     *
     * This method extracts all remaining data that has been successfully received but is outside the main data buffer.
     *
     * @param[out] buffer Variable to hold the remaining recieved data.
     * @param[in] maxBufferSz Maximum size of data that can be held by the buffer variable.
     * @return The size of the data.
     */
    size_t getRemainingBuffer(unsigned char *buffer, size_t maxBufferSz);

    /**
     * @brief Overloading method for __getRemainingBuffer__ with output parameter as vector.
     *
     * Retrieves all remaining data that has been successfully received but is outside the main data buffer.
     *
     * @param[out] buffer Variable to hold the remaining data, provided as a vector.
     * @return The size of the remaining data.
     */
    size_t getRemainingBuffer(std::vector <unsigned char> &buffer);

    /**
     * @brief Retrieves remaining data that has been successfully received but is outside the main data buffer, returning it as a vector.
     *
     * This method retrieves all remaining data that has been successfully received but is not included in the main data buffer, and returns it as a vector.
     *
     * @return std::vector<unsigned char> containing the remaining data that has been successfully received.
     */
    std::vector <unsigned char> getRemainingBufferAsVector();

    /**
     * @brief Performs the operation of sending Socket data.
     *
     * This method sends the specified data to the Socket port.
     *
     * @param[in] buffer Data to be written.
     * @param[in] sz Size of the data to be written.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if the data write operation fails.
     */
    int sendData(const unsigned char *buffer, size_t sz);

    /**
     * @brief Method overloading of `sendData` with input as `const std::vector`.
     *
     * This method sends the specified data to the Socket port. This overload allows the data to be passed as a `const std::vector`.
     *
     * @param[in] buffer Data to be written.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if the data write operation fails.
     */
    int sendData(const std::vector <unsigned char> buffer);

    /**
     * @brief Method overloading of `sendData` with input as `const char*`.
     *
     * This method sends the specified data to the Socket port. This overload allows the data to be passed as a `const char*`.
     *
     * @param[in] buffer Data to be written.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if the data write operation fails.
     */
    int sendData(const char *buffer);

    /**
     * @brief Method overloading of `sendData` with input as `const std::string`.
     *
     * This method sends the specified data to the Socket port. This overload allows the data to be passed as a `const std::string`.
     *
     * @param[in] buffer Data to be written.
     * @return `0` if the operation is successful.
     * @return `1` if the port is not open.
     * @return `2` if the data write operation fails.
     */
    int sendData(const std::string buffer);

    /**
     * @brief Closes the Socket connection between client and server.
     *
     * This function is used to close the currently Socket connection between client and server,
     * ensuring that the port is no longer in use and that any associated system resources
     * are released.
     */
    void closeConnection();

    /**
     * @brief Closes the Socket communication port.
     *
     * This function is used to close the currently Socket communication port,
     * ensuring that the port is no longer in use and that any associated system resources
     * are released.
     */
    void closeSocket();
};

#endif